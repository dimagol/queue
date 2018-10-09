//
// Created by dima on 18/05/18.
//

//    Chunk Length                          Num Of Chunks
//    +--------+--------+--------+------------------+--------+--------+--------+
//    |        |        |        |        ||        |        |        |        |
//    +--------+--------+--------+------------------+--------+--------+---------
//    Chunk Number
//    +--------+--------+--------+--------+
//    |        |        |        |        |
//    +--------+--------+--------+---------


#ifndef TCP_SHMAFKA_SIMPLEMSGHEADER_H
#define TCP_SHMAFKA_SIMPLEMSGHEADER_H

#define SIMPLE_MSG_OFFSET 0
#define SIMPLE_MSG_LEN_LEN 4

#define SIMPLE_MSG_NUMBER_OF_CHUNCKS_OFFSET (SIMPLE_MSG_OFFSET + SIMPLE_MSG_LEN_LEN)
#define SIMPLE_MSG_NUMBER_OF_CHUNCKS_LEN 4

#define SIMPLE_MSG_CHUNK_NUMBER_OFFSET  (SIMPLE_MSG_NUMBER_OF_CHUNCKS_OFFSET + SIMPLE_MSG_NUMBER_OF_CHUNCKS_LEN)
#define SIMPLE_MSG_CHUNK_NUMBER_LEN 4

#define SIMPLE_MSG_CHUNK_ALL_HEADER_LEN (SIMPLE_MSG_CHUNK_NUMBER_OFFSET + SIMPLE_MSG_CHUNK_NUMBER_LEN)

#define SIMPLE_MSG_DATA_OFFSET SIMPLE_MSG_CHUNK_ALL_HEADER_LEN

#endif //TCP_SHMAFKA_SIMPLEMSGHEADER_H
