#include <bits/stdc++.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/un.h>

using namespace std;

struct lis_input {
    int starting_block;
    int number_of_blocks;
    int request_number;
};

int NUMBER_OF_PAGES = 0;
int PAGE_SIZE = 512; // This is in Bytes

inline std::string& rtrim(std::string& s) {
    const char* t = " \t\n\r\f\v";
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from beginning of string (left)
inline std::string& ltrim(std::string& s) {
    const char* t = " \t\n\r\f\v";
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from both ends of string (right then left)
inline std::string& trim(std::string& s) {
    const char* t = " \t\n\r\f\v";
    return ltrim(rtrim(s));
}

class Node {
public:
    Node* next;
    Node* prev;
    int index;
    string data;
};

struct node_presence {
    bool found;
    Node* node;
};

using namespace std;

class Cache_Linked_List {
public:
    int length;
    int cache_access;
    int num_hits;
    int num_misses;
    int MAX_NUM_PAGES;
    map<int, Node*> page_indexes;

    Node* head;
    Node* tail;
    string NODE_DATA;
    Cache_Linked_List();

    Cache_Linked_List(int num_pages) {
        this->length = 0;
        this->head = NULL;
        this->cache_access = 0;
        this->num_hits = 0;
        this->num_misses = 0;
        this->MAX_NUM_PAGES = num_pages;
    }

    ~Cache_Linked_List() {
        Node* next = head;
        Node* cur = NULL;
        while (next != NULL) {
            cur = next;
            next = next->next;
            delete cur;
        }
    }

    void access_cache(int start_block_index, int block_size) {
        // for each block from the start_block_index find the node that matches the index of the given block
        for (int i = start_block_index; i < start_block_index + block_size; i++) {
            int current_block_index = i;
            node_presence node_found = this->get_node(current_block_index);
            //if the block is present
            if (node_found.found) { // The Node Was Found.
                //move it to the head of the linked_list.
                this->hit(node_found.node);
            } else {
                this->miss(current_block_index);
            }
        }
    }

    void hit(Node* node) {
        int node_index = node->index;
        this->del(node);
        this->add_front(node_index);
        this->num_hits++;
        this->cache_access++;
    }

    void miss(int index) {
        //if the linked_list is not greater than max number of pages
        if (this->length < this->MAX_NUM_PAGES) {
            //add it to the head of the linked_list
            this->add_front(index);
        } else {
            //Remove the tail Item from the linked_list and add the current_block to the head of the linked list
            this->remove_last();
            this->add_front(index);
        }
        this->num_misses++;
        this->cache_access++;
    }

    void remove_last() { //Remove the Last Item from the List.
        Node* last_node = this->tail;
        Node* previous_to_last_node = last_node->prev;
        previous_to_last_node->next = NULL;
        this->tail = previous_to_last_node;
        this->length--;
        //: Remove From Map.
        this->remove_node_from_map(last_node->index);
        delete last_node;
    }
    void add_front(int index) {
        Node* node = new Node();
        node->index = index;
        node->next = this->head;
        if (head == NULL) {
            this->head = node;
            this->tail = node;
        } else {
            this->head->prev = node;
            this->head = node;
        }
        //Add to Map.
        this->add_node_to_map(index, node);
        this->length++;
    }

    void add_node_to_map(int index, Node* node) {
        this->page_indexes[index] = node;
    }

    void remove_node_from_map(int index) {
        this->page_indexes.erase(index);
    }

    //Can give Found Node Or Can Give Tail.
    node_presence get_node(int index) {
        //Lookup in the Map to Find the Key.
        node_presence node_found;
        Node* head = this->head;
        node_found.found = false;
        node_found.node = head;
        if (!(this->page_indexes.find(index) == this->page_indexes.end())) {
            node_found.node = this->page_indexes[index];
            node_found.found = true;
        }
        return node_found;
    }

    void add_back(int index) {
        Node* node = new Node();
        node->index = index;
        node->next = NULL;
        if (head == NULL) {
            this->head = node;
            this->tail = node;
        } else {
            this->tail->next = node;
            node->prev = this->tail;
            this->tail = node;
        }
        this->length++;
    }

    void after(Node* a, int value) {
        Node* p = new Node;
        p->index = value;
        p->next = a->next;
        a->next = p;
        p->prev = a;
        this->length++;
    }

    void del(Node* deletion_node) {
        Node* prev_node;
        Node* next_node;
        next_node = deletion_node->next;
        prev_node = deletion_node->prev;
        if (deletion_node == this->head) {
            next_node->prev = NULL;
            this->head = next_node;
        } else if (deletion_node == this->tail) {
            prev_node->next = NULL;
            this->tail = prev_node;
        } else {
            if (prev_node != NULL) {
                prev_node->next = next_node;
            }
            if (next_node != NULL) {
                next_node->prev = prev_node;
            }
        }
        this->remove_node_from_map(deletion_node->index);
        this->length--;
        delete deletion_node;
    }

    void print_reverse() {
        Node* tail = this->tail;
        int i = 1;
        while (tail) {
            std::cout << i << ": " << tail->prev << std::endl;
            tail = tail->prev;
            i++;
        }
    }
    void print_op() {
        Node* head = this->head;
        int i = 1;
        while (head) {
            std::cout << i << ": " << head->index << std::endl;
            if (head == NULL) {
                cout << "Breaking " << endl;
                break;
            }
            head = head->next;
            i++;
        }
    }

    void print_cache_stats() {
        cout << "Number Of Hits : " << this->num_hits << endl;
        cout << "Number Of Misses : " << this->num_misses << endl;
        cout << "Total Cache Visits : " << this->cache_access << endl;
        cout << "Hit Ratio : " << (this->num_hits) << "/" << (this->cache_access) << " = " << (float)(this->num_hits) / (float)(this->cache_access) << endl;
    }
};

class Arc_Window {
public:
    int WINDOW_SIZE;
    int num_hits;
    int num_misses;
    int num_access;
    Arc_Window();
    Cache_Linked_List* frequently_used_lru_top; //Part of Window..
    Cache_Linked_List* frequently_used_lru_bottom;

    Cache_Linked_List* first_time_access_lru_top; //Part of Window..
    Cache_Linked_List* first_time_access_lru_bottom;

    Arc_Window(int window_size) {
        WINDOW_SIZE = window_size;
        first_time_access_lru_top = new Cache_Linked_List(window_size);
        first_time_access_lru_bottom = new Cache_Linked_List(window_size);
        frequently_used_lru_top = new Cache_Linked_List(window_size);
        frequently_used_lru_bottom = new Cache_Linked_List(window_size);
        num_hits = 0;
        num_misses = 0;
        num_access = 0;
    }

    void access_cache(int start_block_index, int block_size) {
        /**
         * Algorithm : twos LRU caches -> frequently used cache and once accessed cache.
         * 
        */

        int adaptation_parameter = 0;
        for (int i = start_block_index; i < start_block_index + block_size; i++) {
            int current_block_index = i;
            node_presence node_top2 = frequently_used_lru_top->get_node(current_block_index);
            node_presence node_top1 = first_time_access_lru_top->get_node(current_block_index); 
            node_presence node_bottom1 = first_time_access_lru_bottom->get_node(current_block_index); 
            node_presence node_bottom2 = frequently_used_lru_bottom->get_node(current_block_index); 
            if(node_top2.found || node_top1.found){ //xt in first_time_access_lru_top || frequently_used_lru_top
            //TODO :Cache Hit In Arc
                if(node_top2.found){
                    //Check if it is not the head. 
                    if(frequently_used_lru_top->head != node_top2.node){
                        frequently_used_lru_top->del(node_top2.node);
                        // move xt to top of frequently_used_lru_top
                        frequently_used_lru_top->add_front(current_block_index);
                    }
                }else{
                    //Doubt : Check if Found in first_time_access_lru_top, should it be removed or not. 
                    first_time_access_lru_top->del(node_top1.node);
                    // move xt to top of frequently_used_lru_top
                    frequently_used_lru_top->add_front(current_block_index);
                }

            }else if(node_bottom1.found){ // found in first_time_access_lru_bottom
                //TODO  : Cache Miss In Arc. 
                adaptation_parameter = adjust_adaptation_rate(adaptation_parameter,false);
                replace_node(false,adaptation_parameter);
                first_time_access_lru_bottom->del(node_bottom1.node);
                frequently_used_lru_top->add_front(current_block_index);
            }else if(node_bottom2.found){ // found in frequently_used_lru_bottom
                //TODO : Cache Miss ON Arc. 
                adaptation_parameter = adjust_adaptation_rate(adaptation_parameter,true);
                replace_node(true,adaptation_parameter);
                frequently_used_lru_bottom->del(node_bottom2.node);
                frequently_used_lru_top->add_front(current_block_index);
            }else{
                //TODO: Cache Miss everywhere. 
                if(first_time_access_lru_bottom->length +first_time_access_lru_top->length == WINDOW_SIZE){
                    if(frequently_used_lru_top->length < WINDOW_SIZE){
                        //Delete LRU page in B1. REPLACE .
                        first_time_access_lru_bottom->remove_last();
                        replace_node(false,adaptation_parameter);

                    }else{
                        first_time_access_lru_top->remove_last();
                    }
                }else if (first_time_access_lru_bottom->length +first_time_access_lru_top->length < WINDOW_SIZE){
                    if(first_time_access_lru_bottom->length +first_time_access_lru_top->length + frequently_used_lru_top->length + frequently_used_lru_bottom->length >= WINDOW_SIZE){
                        if(first_time_access_lru_bottom->length +first_time_access_lru_top->length + frequently_used_lru_top->length + frequently_used_lru_bottom->length == 2*WINDOW_SIZE){
                            frequently_used_lru_bottom->remove_last();
                        }
                        replace_node(false,adaptation_parameter);
                    }
                }else{
                    //This Case Should not be working. 
                    cout << ": This Should Not Be Reached : " << endl;
                }
                //finally, fetch xt to the cache and move it to MRU position in first_time_access_lru_top
                first_time_access_lru_top->add_front(current_block_index);
            }
        }
    }

    int adjust_adaptation_rate(int adaptation_parameter, bool adaptation_param){
        int delta = 0;
        if(adaptation_param){ // Frequency Adaptation
            if(frequently_used_lru_bottom->length >= first_time_access_lru_bottom->length){
                delta = 1;
            }else{
                delta = static_cast<int>(first_time_access_lru_bottom->length/frequently_used_lru_bottom->length);
            }
            return max(adaptation_param-delta,0);
        }else{ // first_time_access Adaptation
            if(first_time_access_lru_bottom->length >= frequently_used_lru_bottom->length){
                delta = 1;
            }else{
                delta = static_cast<int>(frequently_used_lru_bottom->length/first_time_access_lru_bottom->length);
            }
            return min(adaptation_param+delta,WINDOW_SIZE);
        }

    }
    void replace_node(bool frequenty_used_cache_presence,int adaptation_parameter){
        //Fill The Method Here. 
        if((first_time_access_lru_top->length > 0) && ((first_time_access_lru_top->length > adaptation_parameter) || (first_time_access_lru_top->length == adaptation_parameter && frequenty_used_cache_presence))){
            int transfer_index = first_time_access_lru_top->tail->index;    
            first_time_access_lru_top->remove_last();
            first_time_access_lru_bottom->add_front(transfer_index);
        }else{
            int transfer_index = frequently_used_lru_top->tail->index;    
            frequently_used_lru_top->remove_last();
            frequently_used_lru_top->add_front(transfer_index);
        }

    }
};

std::vector<int> split_string_to_int(std::string str, std::string sep) {
    std::stringstream in(str);
    std::vector<int> a;
    int temp;
    while (in >> temp) {
        //cout << temp << endl;
        a.push_back(temp);
    }

    return a;
}

vector<lis_input> retrive_cache_info(string file_path) {
    vector<lis_input> cache_blocks; // For Reduce file creation.
    ifstream file;
    string line;
    file.open(file_path.c_str());
    if (file.is_open()) {
        while (getline(file, line)) {
            string sm_line = line + "\0";
            vector<int> param_splits = split_string_to_int(sm_line, " ");
            lis_input cache_params;
            cache_params.starting_block = (param_splits[0]);
            cache_params.number_of_blocks = (param_splits[1]);
            cache_params.request_number = (param_splits[4]);
            cache_blocks.push_back(cache_params);
        }
    } else {
        cout << "File Didn't Open In Map" << endl;
    }

    return cache_blocks;
}

void test_arc_cache(vector<lis_input> cache_blocks, int cache_num_pages) {
}

void test_lru_cache(vector<lis_input> cache_blocks, int cache_num_pages) {
    Cache_Linked_List* lru_cache = new Cache_Linked_List(cache_num_pages);
    for (int i = 1; i < 11; i++) {
        lru_cache->add_front(i);
    }
    int t = 1;
    Node* head = lru_cache->head;
    while (head) {
        if (head->index == 4) {
            break;
        }
        head = head->next;
        t++;
    }
    node_presence found_node = lru_cache->get_node(5);
    cout << " Found Node 12 : " << found_node.found << endl;
    lru_cache->del(head);
    lru_cache->add_front(4);
    //lru_cache->print_op();
    //cout << "TAIL VALUE " << lru_cache->tail->index << endl;
}

void check_lru_cache(vector<lis_input> cache_blocks, int cache_num_pages) {
    Cache_Linked_List* lru_cache = new Cache_Linked_List(cache_num_pages);
    vector<lis_input>::iterator itr;
    int total = cache_blocks.size();
    for (itr = cache_blocks.begin(); itr < cache_blocks.end(); itr++) {
        lru_cache->access_cache(itr->starting_block, itr->number_of_blocks);
    }
    lru_cache->print_cache_stats();
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Arguements Not Supported" << endl;
        return 0;
    }
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    NUMBER_OF_PAGES = stoi(argv[1]);

    string trace_file_name = argv[2];
    trace_file_name += ".lis";
    vector<lis_input> cache_blocks = retrive_cache_info(trace_file_name);
    check_lru_cache(cache_blocks, NUMBER_OF_PAGES);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "TIME TAKEN BY THE CODE : " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << endl;
    //TODO : Create a Linked list for each of the values in the trace_file.
}