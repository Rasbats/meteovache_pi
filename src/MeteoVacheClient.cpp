/*
 * MeteoVacheClient.cpp
 *
 *  Created on: 10 nov. 2019
 *      Author: ronan
 */

#include <Forecast.h>
#include <zlib.h>
#include "MeteoVacheClient.h"
#include "SpotForecasts.h"

#define MV_CMD_REQUEST_ALL_FORECATS_AT_LOCATION 1

MeteoVacheClient::MeteoVacheClient() {
	serverIpAddr.Hostname(DEFAULT_METEOVACHE_SERVER_NAME);
	serverIpAddr.Service(DEFAULT_METEOVACHE_SERVER_PORT);

	localIpAddr.AnyAddress();
	localIpAddr.Service(0x8000);

	localSocket = new wxDatagramSocket(localIpAddr, wxSOCKET_BLOCK);
	localSocket->SetTimeout(1);
}

MeteoVacheClient::~MeteoVacheClient() {
	delete localSocket;
}

bool MeteoVacheClient::downloadAllForecasts(float latitude, float longitude, SpotForecasts &spotForecast) {
	char requestBuffer[9];
	int nbForecasts;

	requestBuffer[0] = MV_CMD_REQUEST_ALL_FORECATS_AT_LOCATION;
	// TODO : Handle endianess
	*((float*) (requestBuffer + 1)) = latitude;
	*((float*) (requestBuffer + 5)) = longitude;

	localSocket->SendTo(serverIpAddr, requestBuffer, sizeof(requestBuffer));
	localSocket->Read(gzippedResponse, sizeof(gzippedResponse));
	localSocket->LastReadCount();
	uncompressBuffer(gzippedResponse, sizeof(gzippedResponse), serverResponse, sizeof(serverResponse));

	nbForecasts = serverResponse[0];
	spotForecast.Lock();
	spotForecast.Reset(nbForecasts);
	spotForecast.SetPosition(latitude, longitude);

	int dataOffset = 1;
	for (int i = 0; i < nbForecasts; i++) {
		dataOffset += spotForecast.Get(i).readBinary(serverResponse + dataOffset);
	}

	spotForecast.Unlock();

	return (true);
}

unsigned int MeteoVacheClient::uncompressBuffer(void *inputBuffer, unsigned int inputLength, void *outputBuffer, unsigned int outputLength) {
	int err;
	z_stream zStream;

	zStream.zalloc = Z_NULL;
	zStream.zfree = Z_NULL;
	zStream.opaque = Z_NULL;
	zStream.avail_in = 0;
	zStream.next_in = Z_NULL;
	err = inflateInit(&zStream);

	zStream.avail_in = inputLength;
	zStream.next_in = (unsigned char*) inputBuffer;

	zStream.avail_out = outputLength;
	zStream.next_out = (unsigned char*) outputBuffer;

	while (1) {
		err = inflate(&zStream, Z_NO_FLUSH);

		if (err == Z_STREAM_END)
			break;
	}

	err = inflateEnd(&zStream);

	return (outputLength - zStream.avail_out);
}
