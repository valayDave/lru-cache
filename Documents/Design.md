# LRU CACHE Implementation

## Bugs 
- None Detected

## Datastructures
- ```class Node``` : Class which specifies the node in the linked list for the LRU Cache
- ```class Cache_Linked_List``` :
    - Contains the pointer references for the head and tail ```Node``` . This itself can be used as the LRU cache but for each page check, It will have to traverse the complete linked list each time. Thereby to optimise a hashmap is used. 
    - Contains a hashmap ```map<int,*Node> page_indexes``` : which helps check if the node is present in the linked list or not. This adds for the optimisation for the cache to make it faster. Reduces list traversal from O(n) to O(1) by directly identifying the node for the page_id
- ```struct lis_input``` : To store the values of page_indexes from each line in this ```.lis``` file provided. 


## Process Flow 
- The LRU cache (```Cache_Linked_List```) consists of a doubly linked-list which consists of ```Node``` references for the head and tail. It also consists of hashmap (```map<int,*Node> page_indexes```) which contains the reference to the node according to the page id. 
- ```.lis``` file is read and for each line in the file the LRU cache is accessed for the page_ids pertaining to each line.
    - If there is a hit in the cache the page id than the item is removed linked list and placed at the head of the list. 
    - If there is a miss we add the item to the linked-list and also to the hashmap. In this case if the cache is full it removes the last item from the list and the same item from the linked list.
- When the file is completely processed from the cache the stats about the cache are printed. 


## Caching Results. 

| File Name | Cache Size | Hit Ratio  | Hit %  |
|-----------|------------|------------|--------|
| P6.lis    | 1024       | 0.00708153 | 0.71%  |
| P6.lis    | 2048       | 0.00859343 | 0.86%  |
| P6.lis    | 4096       | 0.0109361  | 1.09%  |
| OLTP.lis  | 1024       | 0.332185   | 33.22% |
| OLTP.lis  | 2048       | 0.427774   | 42.78% |
| OLTP.lis  | 4096       | 0.512405   | 51.24% |
| P3.lis    | 1024       | 0.0104928  | 1.05%  |
| P3.lis    | 2048       | 0.0115155  | 1.15%  |
| P3.lis    | 4096       | 0.0131882  | 1.32%  |

## Contributions 

- Discussion of Datastructure and optimisation with William in class. 

## References

1. [Erasing a key in Map Datatype](https://www.geeksforgeeks.org/map-erase-function-in-c-stl/)
2. [Searching Values in Map Datatype](https://www.geeksforgeeks.org/searching-map-using-stdmap-functions-c/)

