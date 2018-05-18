//
// Created by dima on 18/05/18.
//

#ifndef TCP_SHMAFKA_LISTOFBUFFERLISTS_H
#define TCP_SHMAFKA_LISTOFBUFFERLISTS_H


#include "BuffersList.h"

class ListOfBufferLists {
public:
    ListOfBufferLists() :head(nullptr),
                   tail(nullptr),
                   len(0){}

    explicit ListOfBufferLists(BuffersList *buffersList) :
    head(buffersList),
            tail(buffersList),
            len(1) {
        if(buffersList->nextList != nullptr){
            LOG_ERROR("logic issue...")
        }
    }

    virtual uint32_t append(BuffersList *buffersList){
        if(buffersList->nextList != nullptr){
            LOG_ERROR("logic issue...")
        }
        if(tail == nullptr){
            tail = buffersList;
            head = buffersList;
            len = 1;
        } else {
            tail->nextList = buffersList;
            tail = buffersList;
            len++;
        }

        return len;
    }

    virtual BuffersList * get_head() {
        return head;
    }

    virtual BuffersList * get_tail() {
        return tail;
    }


    virtual BuffersList * remove_head(){
        BuffersList * tmp = head;
        head = head->nextList;
        len--;
        return tmp;
    }

    virtual uint32_t get_len(){
        return len;
    }

    virtual ~ListOfBufferLists() {
    }

private:

    BuffersList * head;
    BuffersList * tail;
    uint32_t len;
};


#endif //TCP_SHMAFKA_LISTOFBUFFERLISTS_H
