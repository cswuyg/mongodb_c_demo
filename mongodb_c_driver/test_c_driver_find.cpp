/*
* 测试mongodb集群的tag查询
* g++ test_c_driver_find.cpp -I./mongodb-c-driver/include/libbson-1.0 -I./mongodb-c-driver/include/libmongoc-1.0 -L./mongodb-c-driver/lib -lmongoc-1.0 -lpthread -lrt  -lbson-1.0 -lssl -lcrypto -o demo
* Authors: cswuyg
* Date:    2015/12/29 
*/
#include <bson.h>
#include <mongoc.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <ctime>
#include <fstream>

const char* const K_URI = "mongodb://xxxxxhost:27030/?w=1&readPreference=nearest&readPreferenceTags=location:bj";

class HighTimer{
    public:
        HighTimer(){};
        ~HighTimer(){};

        void start(){
                    ::gettimeofday(&_start_tv, NULL);
                }
        double end() {
                    ::gettimeofday(&_end_tv, NULL);
                    return cost_time();
                }
        double cost_time() {
                    uint64_t start = _start_tv.tv_sec * (uint64_t)1000000 + _start_tv.tv_usec;
                    uint64_t end = _end_tv.tv_sec * (uint64_t)1000000 + _end_tv.tv_usec;
                    return end - start;
                }

    private:
        struct timeval _start_tv;
        struct timeval _end_tv;
};

void write_file(const std::string& info) {
    std::ofstream ofs;
    ofs.open("tmp.txt");
    ofs << info;
    ofs.close();
}

void test_query(mongoc_collection_t* collection, mongoc_uri_t* uri) {
    HighTimer cost_time;
    cost_time.start();
    const mongoc_read_prefs_t* read_prefs = mongoc_uri_get_read_prefs_t(uri);
    bson_t* query = bson_new();
    BSON_APPEND_UTF8(query, "name", "cswuyg"); 

    //bson_error_t error;
    //int64_t count = mongoc_collection_count(collection, MONGOC_QUERY_SLAVE_OK, query, 0, 0, read_prefs, &error);
    //std::cout << count << std::endl;

    mongoc_cursor_t* cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 1, 0, query, NULL, read_prefs);
    const bson_t* doc;
    while (mongoc_cursor_next(cursor, &doc)) {
        char* str = bson_as_json(doc, NULL);
        write_file(str);
        bson_free(str);
    }

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    std::cout << "query cost time = " << cost_time.end() << std::endl;
}

void mongo_query(){
    mongoc_uri_t* uri = mongoc_uri_new(K_URI);
    mongoc_client_t* client = mongoc_client_new_from_uri(uri);
	mongoc_collection_t* collection = mongoc_client_get_collection(client, "myapp", "myuser");
	test_query(collection, uri);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy (client);
}

int main (){
	mongoc_init ();
	mongo_query();
	return 0;
}
