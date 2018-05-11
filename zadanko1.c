#include <stdio.h>
#include <stdlib.h>

#define pointer(T)     typeof(T *)
#define dereference(T) typeof(*(T))
#define array(T, N)    typeof(T [N])

#define Map(T, N)                           \
({  typeof(T) _T = 0;                       \
    typeof(N) _N = 0;                       \
    _construct(typename(_T), typename(_N)); \
})

#define Add(thiscall, key, value)                                       \
({  int v = validate(thiscall, typename(key), typename(value));         \
    if(v < 0)                                                           \
    {                                                                   \
      fprintf(stderr, "map not initialized or corrupted\n");            \
      exit(-1);                                                         \
    }                                                                   \
    if(v > 0)                                                           \
    {                                                                   \
      fprintf(stderr, "wrong parameter type\n");                        \
      exit(1);                                                          \
    }                                                                   \
    typeof(key)   _key         = key;                                   \
    typeof(value) _value       = value;                                 \
    dereference(thiscall) _map = *thiscall;                             \
                                                                        \
    _map.method->add(&_key, &_value, typename(key), typename(value));   \
})

#define TYPE_0  error
#define TYPE_1  _Bool
#define TYPE_2  unsigned char
#define TYPE_3  char
#define TYPE_4  signed char
#define TYPE_5  short int
#define TYPE_6  unsigned short int
#define TYPE_7  int
#define TYPE_8  unsigned int
#define TYPE_9  long int
#define TYPE_10 unsigned long int
#define TYPE_11 long long int
#define TYPE_12 unsigned long long int
#define TYPE_13 float
#define TYPE_14 double
#define TYPE_15 long double
#define TYPE_16 char*

#define TYPE_SELECT(__typeid) TYPE_ ## __typeid

#define typename(x) _Generic                    \
    ((x),                                       \
         _Bool: 1,           unsigned char: 2,  \
          char: 3,             signed char: 4,  \
     short int: 5,      unsigned short int: 6,  \
           int: 7,            unsigned int: 8,  \
      long int: 9,       unsigned long int: 10, \
 long long int: 11, unsigned long long int: 12, \
         float: 13,                 double: 14, \
   long double: 15,                char * : 16, \
       default: -1  \
     )

typedef struct
{
  int size;
  int keyType;
  int valueType;
}
assocTable_values;

struct assocTable_methods
{
  void (*add)    (void *, void *, int, int);
  void (*delete) (void *, void *, int, int);
  void (*find)   (void *, void *, int, int);
};

typedef struct
{
  const struct assocTable_methods *method;
  assocTable_values               *value;
}
assocTable;

void       _add       (void       *incomingKey, void* incomingValue, int keyType, int keyValue);
void       _delete    (void       *incomingKey, void* incomingValue, int keyType, int keyValue);
void       _find      (void       *incomingKey, void* incomingValue, int keyType, int keyValue);
int        validate   (assocTable *thiscall,    int   keyType,       int valueType);
assocTable _construct (int         keyType,     int   valueType);

static const struct assocTable_methods
  assocTable_static_methods =
  {
    _add,
    _delete,
    _find
  };

int main (void)
{
  assocTable mapa = Map(char*, int);
  Add(&mapa, "h", 1);

  return 0;
}

assocTable _construct(int keyType, int valueType)
{
  assocTable Map;

  Map.method = &assocTable_static_methods;
  Map.value  = (assocTable_values  *)malloc(sizeof(assocTable_values ));

  Map.value->size      = 0;
  Map.value->keyType   = keyType;
  Map.value->valueType = valueType;

  return Map;
}

void _add(void *incomingKey, void* incomingValue, int keyType, int keyValue)
{
  printf("_add\n");
  //this isn't working yet!!!
  if(keyType == 7)  TYPE_SELECT(7) x = *(TYPE_SELECT(7) )incomingKey;
  if(keyType == 16) TYPE_SELECT(16) x =  (TYPE_SELECT(16))incomingKey;

}

void _delete(void *incomingKey, void* incomingValue, int keyType, int keyValue)
{
  printf("_delete\n");
}

void _find(void *incomingKey, void* incomingValue, int keyType, int keyValue)
{
  printf("_find\n");
}

int validate(assocTable *thiscall, int keyType, int valueType)
{
  if(thiscall                    == NULL) return -1;
  if(thiscall->method            == NULL) return -2;
  if(thiscall->value             == NULL) return -3;
  if(thiscall->value->keyType    != keyType) return 1;
  if(thiscall->value->valueType  != valueType) return 2;

  return 0;
}
