/*
  Copyright (c) 2009 Dave Gamble

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef cJSON__h
#define cJSON__h

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>

/* cJSON Types: 
//chensk 2019-11-28:JSON数据元素类型
布尔型：true false
空型：NULL
数值
字符串
数组
对象
*/
#define cJSON_False  (1 << 0)
#define cJSON_True   (1 << 1)
#define cJSON_NULL   (1 << 2)
#define cJSON_Number (1 << 3)
#define cJSON_String (1 << 4)
#define cJSON_Array  (1 << 5)
#define cJSON_Object (1 << 6)

#define cJSON_IsReference 256
#define cJSON_StringIsConst 512

/* The cJSON structure: cJson对象结构体
//chensk 2019-11-28:结构体元素说明
next:下一个对象节点地址
prev：上一个节点对象地址  json对象为双向链表
child:子节点地址，如果是一个object或array的话，child为第一个儿子的指针

type:对象数据类型

valuestring：数据对象为字符串是，存放字符串位置，不是字符串是此处为NULL

valueint：数据对象类型为数字时，整型数值
valuedouble：数据对象类型为数字时，float整型数值

string:json对象的名称  
*/
typedef struct cJSON
{
    /* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
    struct cJSON *next;
    struct cJSON *prev;
    /* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */
    struct cJSON *child;

    /* The type of the item, as above. */
    int type;

    /* The item's string, if type==cJSON_String */
    char *valuestring;
    /* The item's number, if type==cJSON_Number */
    int valueint;
    /* The item's number, if type==cJSON_Number */
    double valuedouble;

    /* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
    char *string;
} cJSON;

//chensk 2019-11-28:cjson的钩子函数

typedef struct cJSON_Hooks
{
      void *(*malloc_fn)(size_t sz);
      void (*free_fn)(void *ptr);
} cJSON_Hooks;

/* Supply malloc, realloc and free functions to cJSON */
/**************************************************************************
describe:初始化钩子函数
         如果用户指定hook，则使用用户自定义的内存管理函数
         否则使用系统默认的malloc和free函数
para:hooks 用户自定义申请和释放内存的接口
returnval:void
author:chensk  2019-11-28
**************************************************************************/
extern void cJSON_InitHooks(cJSON_Hooks* hooks);


/* Supply a block of JSON, and this returns a cJSON object you can interrogate. Call cJSON_Delete when finished. */
/**************************************************************************
describe:cJSON解析，将输入的字符串json信息解析为结构体数据
para:
     此函数是一个二次封装函数，其内部为cJSON_ParseWithOpts()函数，该函数用于提取更多的解析选项，如果需要，最后返回解析结束的位置
returnval:
author:chensk  2019-11-28
**************************************************************************/
extern cJSON *cJSON_Parse(const char *value);

/* Render a cJSON entity to text for transfer/storage. Free the char* when finished. */
//chensk 2019-11-28:cjson打印就是从根对象的结构体开始遍历，得到每个item结点的名称和数据，并经过处理成特定的cjson字符串的输出形式。
//处理成的字符串里有\t\n这类的格式,内存为动态实时分配
extern char  *cJSON_Print(const cJSON *item);

/* Render a cJSON entity to text for transfer/storage without any formatting. Free the char* when finished. */
//chensk 2019-11-28:处理成的字符串里没有\t\n这类的格式
extern char  *cJSON_PrintUnformatted(const cJSON *item);

/* Render a cJSON entity to text using a buffered strategy. prebuffer is a guess at the final size. guessing well reduces reallocation. fmt=0 gives unformatted, =1 gives formatted */
//使用缓冲策略将cJSON实体呈现为文本。
extern char *cJSON_PrintBuffered(const cJSON *item, int prebuffer, int fmt);

/* Render a cJSON entity to text using a buffer already allocated in memory with length buf_len. Returns 1 on success and 0 on failure. */
//使用长度为buf_len的内存中已分配的缓冲区将cJSON实体呈现为文本
extern int cJSON_PrintPreallocated(cJSON *item, char *buf, const int len, const int fmt);
/* Delete a cJSON entity and all subentities. */
extern void   cJSON_Delete(cJSON *c);

/* Returns the number of items in an array (or object). */
extern int	  cJSON_GetArraySize(const cJSON *array);
/* Retrieve item number "item" from array "array". Returns NULL if unsuccessful. */
extern cJSON *cJSON_GetArrayItem(const cJSON *array, int item);
/* Get item "string" from object. Case insensitive. */
extern cJSON *cJSON_GetObjectItem(const cJSON *object, const char *string);
extern int cJSON_HasObjectItem(const cJSON *object, const char *string);
/* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when cJSON_Parse() returns 0. 0 when cJSON_Parse() succeeds. */
extern const char *cJSON_GetErrorPtr(void);

/* These calls create a cJSON item of the appropriate type. */
//chensk 2019-11-28:创建json节点对象，设置节点类型
extern cJSON *cJSON_CreateNull(void);  //创建空型节点
extern cJSON *cJSON_CreateTrue(void);  //创建布尔型，ture
extern cJSON *cJSON_CreateFalse(void); //创建布尔型节点，false
extern cJSON *cJSON_CreateBool(int b);  //创建布尔型节点，由参数指令ture或者false

//chensk 2019-11-28:创建节点对象，并填充节点数值
extern cJSON *cJSON_CreateNumber(double num);  //创建数字型节点
extern cJSON *cJSON_CreateString(const char *string);//创建字符串型节点，字符串型需要单独申请字符串空间
extern cJSON *cJSON_CreateArray(void);//创建数组型节点
extern cJSON *cJSON_CreateObject(void);//创建对象型节点

/* These utilities create an Array of count items. */
//chensk 2019-11-28:创建数组型对象节点,为创建子链表，child会赋值，child指向子链表的链表头，即首个元素地址
extern cJSON *cJSON_CreateIntArray(const int *numbers, int count); //创建int项节点
extern cJSON *cJSON_CreateFloatArray(const float *numbers, int count);
extern cJSON *cJSON_CreateDoubleArray(const double *numbers, int count);
extern cJSON *cJSON_CreateStringArray(const char **strings, int count);

/* Append item to the specified array/object. */
//增加无string对象节点到数组
extern void cJSON_AddItemToArray(cJSON *array, cJSON *item);
//增加有string对象节点到json对象
extern void	cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item);

/* Use this when string is definitely const (i.e. a literal, or as good as), and will definitely survive the cJSON object.
 * WARNING: When this function was used, make sure to always check that (item->type & cJSON_StringIsConst) is zero before
 * writing to `item->string` */
extern void	cJSON_AddItemToObjectCS(cJSON *object, const char *string, cJSON *item);
/* Append reference to item to the specified array/object. Use this when you want to add an existing cJSON to a new cJSON, but don't want to corrupt your existing cJSON. */
extern void cJSON_AddItemReferenceToArray(cJSON *array, cJSON *item);
extern void	cJSON_AddItemReferenceToObject(cJSON *object, const char *string, cJSON *item);

/**************************************************************************
describe:cJSON的删除;删除节点很简单， 先删除儿子，然后清理内存即可
        总结一下就是对于 object 和 array 需要先删除儿子，然后删除自己
        对于 字符串， 需要先释放字符串的内存， 再释放自己这块内存
        对于其他节点，直接释放自己这块内存
para:
returnval:
author:chensk  2019-11-28
**************************************************************************/

/*
Detach 又是什么东西呢？
我们把一个节点从 json 树中删除， 但是不释放内存，而是先保留这个节点的指针， 这
样储存在这个节点的信息都保留了下来。
接下来我们就可以做很多事了， 合适的时候添加到其他对象中， 合适的时候释放内存
比如上面的 delete 函数， 就需要真实的删除了， 这个时候我们删除即可。
而 detach 实现也比较简单， 只是少了一步删除操作。

*/

/* Remove/Detatch items from Arrays/Objects. */
extern cJSON *cJSON_DetachItemFromArray(cJSON *array, int which);
extern void   cJSON_DeleteItemFromArray(cJSON *array, int which);
extern cJSON *cJSON_DetachItemFromObject(cJSON *object, const char *string);
extern void   cJSON_DeleteItemFromObject(cJSON *object, const char *string);

/* Update array items. */
extern void cJSON_InsertItemInArray(cJSON *array, int which, cJSON *newitem); /* Shifts pre-existing items to the right. */
extern void cJSON_ReplaceItemInArray(cJSON *array, int which, cJSON *newitem);
extern void cJSON_ReplaceItemInObject(cJSON *object,const char *string,cJSON *newitem);

/* Duplicate a cJSON item */
extern cJSON *cJSON_Duplicate(const cJSON *item, int recurse);
/* Duplicate will create a new, identical cJSON item to the one you pass, in new memory that will
need to be released. With recurse!=0, it will duplicate any children connected to the item.
The item->next and ->prev pointers are always zero on return from Duplicate. */

/* ParseWithOpts allows you to require (and check) that the JSON is null terminated, and to retrieve the pointer to the final byte parsed. */
/* If you supply a ptr in return_parse_end and parsing fails, then return_parse_end will contain a pointer to the error. If not, then cJSON_GetErrorPtr() does the job. */
extern cJSON *cJSON_ParseWithOpts(const char *value, const char **return_parse_end, int require_null_terminated);

extern void cJSON_Minify(char *json);

/* Macros for creating things quickly. */
#define cJSON_AddNullToObject(object,name) cJSON_AddItemToObject(object, name, cJSON_CreateNull()) // 创建一个string值为name的cJSON_Null节点，并添加到object
#define cJSON_AddTrueToObject(object,name) cJSON_AddItemToObject(object, name, cJSON_CreateTrue()) // 创建一个string值为name的cJSON_True节点，并添加到object
#define cJSON_AddFalseToObject(object,name) cJSON_AddItemToObject(object, name, cJSON_CreateFalse())// 创建一个string值为name的cJSON_False节点，并添加到object
// 创建一个string值为name的cJSON_CreateBool节点，并添加到object。b非0为cJSON_True，0为cJSON_False。
#define cJSON_AddBoolToObject(object,name,b) cJSON_AddItemToObject(object, name, cJSON_CreateBool(b))
// 创建一个string值为name，valuedouble为n，valueint为（int）n的cJSON_Number节点，并添加到object。
#define cJSON_AddNumberToObject(object,name,n) cJSON_AddItemToObject(object, name, cJSON_CreateNumber(n))
// 创建一个string值为name，valuestring为s的cJSON_String节点，并添加到object。
#define cJSON_AddStringToObject(object,name,s) cJSON_AddItemToObject(object, name, cJSON_CreateString(s))

/* When assigning an integer value, it needs to be propagated to valuedouble too. */
#define cJSON_SetIntValue(object,val) ((object) ? (object)->valueint = (object)->valuedouble = (val) : (val))
#define cJSON_SetNumberValue(object,val) ((object) ? (object)->valueint = (object)->valuedouble = (val) : (val))

/* Macro for iterating over an array */
#define cJSON_ArrayForEach(pos, head) for(pos = (head)->child; pos != NULL; pos = pos->next)

#ifdef __cplusplus
}
#endif

#endif
