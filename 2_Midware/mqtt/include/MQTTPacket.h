/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef MQTTPACKET_H_
#define MQTTPACKET_H_

#if defined(__cplusplus) /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

enum errors
{
	MQTTPACKET_BUFFER_TOO_SHORT = -2,
	MQTTPACKET_READ_ERROR = -1,
	MQTTPACKET_READ_COMPLETE,
};

// 数据中的高四位？
enum msgTypes
{
	CONNECT = 1, CONNACK, PUBLISH, PUBACK, PUBREC, PUBREL,
	PUBCOMP, SUBSCRIBE, SUBACK, UNSUBSCRIBE, UNSUBACK,
	PINGREQ, PINGRESP, DISCONNECT
};

/**
 * Bitfields for the MQTT header byte.
 */
typedef union
{
	unsigned char byte;	                /**< the whole byte */
#if defined(REVERSED)
	struct
	{
		unsigned int type : 4;			/**< message type nibble */
		unsigned int dup : 1;				/**< DUP flag bit */
		unsigned int qos : 2;				/**< QoS value, 0, 1 or 2 */
		unsigned int retain : 1;		/**< retained flag bit */
	} bits;
#else
	struct
	{
		unsigned int retain : 1;		/**< retained flag bit */
		unsigned int qos : 2;				/**< QoS value, 0, 1 or 2 */
		unsigned int dup : 1;				/**< DUP flag bit */
		unsigned int type : 4;			/**< message type nibble */
	} bits;
#endif
} MQTTHeader;

typedef struct
{
	int len;
	char* data;
} MQTTLenString;

typedef struct
{
	char* cstring;
	MQTTLenString lenstring;
} MQTTString;

#define MQTTString_initializer {NULL, {0, NULL}}

int MQTTstrlen(MQTTString mqttstring);

#include "MQTTConnect.h"
#include "MQTTPublish.h"
#include "MQTTSubscribe.h"
#include "MQTTUnsubscribe.h"

int MQTTSerialize_ack(unsigned char* buf, int buflen, unsigned char type,\
		unsigned short packetid, unsigned char qos, unsigned char dup);
int MQTTDeserialize_ack(unsigned char* packettype, unsigned char* dup, unsigned short* packetid, unsigned char* buf, int buflen);

int MQTTPacket_len(int rem_len);
int MQTTPacket_equals(MQTTString* a, char* b);

int MQTTPacket_encode(unsigned char* buf, int length);
int MQTTPacket_decode(int (*getcharfn)(unsigned char*, int), int* value);
int MQTTPacket_decodeBuf(unsigned char* buf, int* value);

int readInt(unsigned char** pptr);
char readChar(unsigned char** pptr);
void writeChar(unsigned char** pptr, char c);
void writeInt(unsigned char** pptr, int anInt);
int readMQTTLenString(MQTTString* mqttstring, unsigned char** pptr, unsigned char* enddata);
void writeCString(unsigned char** pptr, const char* string);
void writeMQTTString(unsigned char** pptr, MQTTString mqttstring);

int MQTTPacket_read(unsigned char* buf, int buflen, int (*getfn)(unsigned char*, int));

char* MQTTPacket_toString(char* strbuf, int strbuflen, unsigned char* buf, int buflen);

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif


#endif /* MQTTPACKET_H_ */
