#include "SimpleModbusSlave.h"

#define BUFFER_SIZE 128

// frame[] is used to recieve and transmit packages.
// The maximum serial ring buffer size is 128
uint8_t frame[BUFFER_SIZE];
uint16_t holdingRegsSize; // size of the register array
uint8_t broadcastFlag;
uint8_t slaveID;
uint8_t function;
uint16_t errorCount;
uint16_t T1_5; // inter character time out
uint16_t T3_5; // frame delay

// function definitions
void exceptionResponse(UART_HandleTypeDef *huartX, unsigned char exception);
uint16_t calculateCRC(uint8_t bufferSize);
void sendPacket(UART_HandleTypeDef *huartX, uint8_t bufferSize);
void delayMicroseconds(uint16_t us);
uint8_t UARTX_Available(UART_HandleTypeDef *huartX);

extern TIM_HandleTypeDef htim7;
<<<<<<< Updated upstream
=======
extern uint8_t frame[BUFFER_SIZE];
extern uint8_t ModbusFrame[BUFFER_SIZE];
extern uint16_t bufferIdx;
>>>>>>> Stashed changes

uint16_t modbus_update(UART_HandleTypeDef *huartX, uint16_t *holdingRegs)
{
	uint8_t bufferIdx = 0;
	uint8_t overflow = 0;
    
    uint8_t rcv_id;
    uint16_t rcv_crc;

    uint16_t startingAddress, no_of_registers, maxData;

    uint8_t index;
    uint8_t address;
    uint16_t crc16;

    uint8_t noOfBytes, responseFrameSize;
    uint16_t temp;
    uint16_t regStatus;

    while (UARTX_Available(huartX)) {
        // The maximum number of bytes is limited to the serial buffer size of 128 bytes
        // If more bytes is received than the BUFFER_SIZE the overflow flag will be set and the
        // serial buffer will be red untill all the data is cleared from the receive buffer.
        if (overflow)
        	temp = (uint8_t)(huartX->Instance->DR & 0x00FF);
        else {
            if (bufferIdx == BUFFER_SIZE)
                overflow = 1;
            frame[bufferIdx] = (uint8_t)(huartX->Instance->DR & 0x00FF);
			bufferIdx++;
        }
        delayMicroseconds(T1_5); // inter character time out
    }

    // If an overflow occurred increment the errorCount
    // variable and return to the main sketch without
    // responding to the request i.e. force a timeout
    if (overflow)
        return errorCount++;

    // The minimum request packet is 8 bytes for function 3 & 16
    if (bufferIdx > 6) {
<<<<<<< Updated upstream
=======
//
//    	memcpy(frame, ModbusFrame, BUFFER_SIZE);
>>>>>>> Stashed changes
        rcv_id = frame[0];

        broadcastFlag = 0;

        if (rcv_id == 0)
            broadcastFlag = 1;

        if (rcv_id == slaveID || broadcastFlag) { // if the recieved ID matches the slaveID or broadcasting id (0), continue
            rcv_crc = ((frame[bufferIdx - 2] << 8) | frame[bufferIdx - 1]); // combine the crc Low & High bytes
            if (calculateCRC(bufferIdx - 2) == rcv_crc) { // if the calculated crc matches the recieved crc continue
                function = frame[1];
                startingAddress = ((frame[2] << 8) | frame[3]); // combine the starting address bytes
                no_of_registers = ((frame[4] << 8) | frame[5]); // combine the number of register bytes
                maxData = startingAddress + no_of_registers;

                // broadcasting is not supported for function 3
                if (!broadcastFlag && (function == 3)) {
                    if (startingAddress < holdingRegsSize) { // check exception 2 ILLEGAL DATA ADDRESS
                        if (maxData <= holdingRegsSize) { // check exception 3 ILLEGAL DATA VALUE
                            noOfBytes = no_of_registers * 2;
                            responseFrameSize = 5 + noOfBytes; // ID, function, noOfBytes, (dataLo + dataHi) * number of registers, crcLo, crcHi
                            frame[0] = slaveID;
                            frame[1] = function;
                            frame[2] = noOfBytes;
                            address = 3; // PDU starts at the 4th byte

                            for (index = startingAddress; index < maxData; index++) {
                                temp = holdingRegs[index];
                                frame[address] = temp >> 8; // split the register into 2 bytes
                                address++;
                                frame[address] = temp & 0xFF;
                                address++;
                            }

                            crc16 = calculateCRC(responseFrameSize - 2);
                            frame[responseFrameSize - 2] = crc16 >> 8; // split crc into 2 bytes
                            frame[responseFrameSize - 1] = crc16 & 0xFF;
                            sendPacket(huartX, responseFrameSize);
                        } else
                            exceptionResponse(huartX, 3); // exception 3 ILLEGAL DATA VALUE
                    } else
                        exceptionResponse(huartX, 2); // exception 2 ILLEGAL DATA ADDRESS
                } else if (function == 6) {
                    if (startingAddress < holdingRegsSize) { // check exception 2 ILLEGAL DATA ADDRESS
                        startingAddress = ((frame[2] << 8) | frame[3]);
                        regStatus = ((frame[4] << 8) | frame[5]);
                        unsigned char responseFrameSize = 8;

                        holdingRegs[startingAddress] = regStatus;

                        crc16 = calculateCRC(responseFrameSize - 2);
                        frame[responseFrameSize - 2] = crc16 >> 8; // split crc into 2 bytes
                        frame[responseFrameSize - 1] = crc16 & 0xFF;
                        sendPacket(huartX, responseFrameSize);
                    } else
                        exceptionResponse(huartX, 2); // exception 2 ILLEGAL DATA ADDRESS
                } else if (function == 16) {
                    // check if the recieved number of bytes matches the calculated bytes minus the request bytes
                    // id + function + (2 * address bytes) + (2 * no of register bytes) + byte count + (2 * CRC bytes) = 9 bytes
                    if (frame[6] == (bufferIdx - 9)) {
                        if (startingAddress < holdingRegsSize) { // check exception 2 ILLEGAL DATA ADDRESS
                            if (maxData <= holdingRegsSize) { // check exception 3 ILLEGAL DATA VALUE
                                address = 7; // start at the 8th byte in the frame

                                for (index = startingAddress; index < maxData; index++) {
                                    holdingRegs[index] = ((frame[address] << 8) | frame[address + 1]);
                                    address += 2;
                                }

                                // only the first 6 bytes are used for CRC calculation
                                crc16 = calculateCRC(6);
                                frame[6] = crc16 >> 8; // split crc into 2 bytes
                                frame[7] = crc16 & 0xFF;

                                // a function 16 response is an echo of the first 6 bytes from the request + 2 crc bytes
                                if (!broadcastFlag) // don't respond if it's a broadcast message
                                    sendPacket(huartX, 8);
                            } else
                                exceptionResponse(huartX, 3); // exception 3 ILLEGAL DATA VALUE
                        } else
                            exceptionResponse(huartX, 2); // exception 2 ILLEGAL DATA ADDRESS
                    } else
                        errorCount++; // corrupted packet
                } else
                    exceptionResponse(huartX, 1); // exception 1 ILLEGAL FUNCTION
            } else // checksum failed
                errorCount++;
        } // incorrect id
    } else if (bufferIdx > 0 && bufferIdx < 8)
        errorCount++; // corrupted packet

    return errorCount;
}

void exceptionResponse(UART_HandleTypeDef *huartX, unsigned char exception)
{
    errorCount++; // each call to exceptionResponse() will increment the errorCount
    if (!broadcastFlag) { // don't respond if its a broadcast message
        frame[0] = slaveID;
        frame[1] = (function | 0x80); // set the MSB bit high, informs the master of an exception
        frame[2] = exception;
        unsigned int crc16 = calculateCRC(3); // ID, function + 0x80, exception code == 3 bytes
        frame[3] = crc16 >> 8;
        frame[4] = crc16 & 0xFF;
        sendPacket(huartX, 5); // exception response is always 5 bytes ID, function + 0x80, exception code, 2 bytes crc
    }
}

void modbus_configure(UART_HandleTypeDef *huartX, uint8_t _slaveID, uint16_t _holdingRegsSize, uint8_t _lowLatency)
{
    slaveID = _slaveID;
    uint32_t _baud = huartX->Init.BaudRate;

    // Modbus states that a baud rate higher than 19200 must use a fixed 750 us
    // for inter character time out and 1.75 ms for a frame delay.
    // For baud rates below 19200 the timeing is more critical and has to be calculated.
    // E.g. 9600 baud in a 10 bit packet is 960 characters per second
    // In milliseconds this will be 960characters per 1000ms. So for 1 character
    // 1000ms/960characters is 1.04167ms per character and finaly modbus states an
    // intercharacter must be 1.5T or 1.5 times longer than a normal character and thus
    // 1.5T = 1.04167ms * 1.5 = 1.5625ms. A frame delay is 3.5T.
    // Added experimental low latency delays. This makes the implementation
    // non-standard but practically it works with all major modbus master implementations.

    if (_baud == 1000000 && _lowLatency) {
        T1_5 = 1;
        T3_5 = 10;
    } else if (_baud >= 115200 && _lowLatency) {
        T1_5 = 75;
        T3_5 = 175;
    } else if (_baud > 19200) {
        T1_5 = 750;
        T3_5 = 1750;
    } else {
        T1_5 = 15000000/_baud; // 1T * 1.5 = T1.5
        T3_5 = 35000000/_baud; // 1T * 3.5 = T3.5
    }

    holdingRegsSize = _holdingRegsSize;
    errorCount = 0; // initialize errorCount
}

uint16_t calculateCRC(uint8_t bufferSize){
	unsigned int temp, temp2, flag;
	uint8_t i, j;
	temp = 0xFFFF;
	for (i = 0; i < bufferSize; i++){
		temp = temp ^ frame[i];
		for (j = 0; j < 8; j++){
			flag = temp & 0x0001;
			temp >>= 1;
			if (flag) temp ^= 0xA001;
    }
  }
	// Reverse byte order.
	temp2 = temp >> 8;
	temp = (temp << 8) | temp2;
	temp &= 0xFFFF;
	return temp; // the returned value is already swapped - crcLo byte is first & crcHi byte is last
}

void sendPacket(UART_HandleTypeDef *huartX, uint8_t bufferSize)
{
	HAL_UART_Transmit_IT(huartX, frame, bufferSize);

	// allow a frame delay to indicate end of transmission
	delayMicroseconds(T3_5);
}


uint8_t UARTX_Available(UART_HandleTypeDef *huartX){
	return (uint8_t)(__HAL_UART_GET_FLAG(huartX, UART_FLAG_RXNE));
}

void delayMicroseconds(uint16_t us){
	__HAL_TIM_SET_COUNTER(&htim7,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim7) < us);  // wait for the counter to reach the us input in the parameter
}
