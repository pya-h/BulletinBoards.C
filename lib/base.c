#include "base.h"
#include <string.h>
#include <stdarg.h>

short prepareFolder(char folder[], short insideData)
{
    // creates a folder if not exists
    if (insideData)
    {
        char path[MAX_FILENAME_LENGTH] = {0};
        sprintf(path, "%s/%s", FOLDER_DATA, folder);
        return !_mkdir(path);
    }

    return !_mkdir(folder);
}

long MIN(const long a, const long b)
{
    return a <= b ? a : b;
}
long MAX(const long a, const long b)
{
    return a >= b ? a : b;
}
short fileExists(string filename)
{
    FILE *reader = fopen(filename, "r");
    short readOK = reader != NULL;
    fclose(reader);
    return readOK;
}

void getLine(string dest, string inputMessage)
{
    // sometimes the app may has been executed a scanf before gets
    // the ['\n'] next line character may remain in input stream
    // since gets will store next line character too, ['\n'] is a valid string input for gets and
    // it results in gets input skip!
    dest[0] = '\0';
    while (!dest[0])
    {
        printf("\n%s", inputMessage);
        gets(dest);
        removeNextlineCharacter(dest); // remove next line from end of string
    }
}

void removeNextlineCharacter(string str)
{
    // remove the next line character ['\n'] at the end of string (if exists)
    size_t len = strlen(str);
    while (len > 0 && str[len - 1] == '\n')
    {
        str[--len] = '\0'; // Replace '\n' with '\0'
    }
}

string trimColumnValue(string col)
{
    // remove both sides of string from \n character and \" character
    for (size_t i = 1, l = strlen(col); i < l && (!col[l - i] ||
                                                  col[i - 1] == '\"' || col[l - i] == '\n');
         col[l - i] = '\0', i++);
    while (*col == '\"')
        col++; // remove first letter
    return col;
}

string tranctuateText(string text)
{
    const int len = MIN(strlen(text), TRANCTUATE_LENGTH);
    string shortText = String(len);
    for (int i = 0; i < len; shortText[i] = text[i], i++)
        ;
    return shortText;
}

void Free(short count, ...)
{
    va_list pointers;
    va_start(pointers, count);
    while (--count >= 0)
    {
        void *trash = va_arg(pointers, void *);
        free(trash);
    }
    va_end(pointers);
    free(pointers);
}

string encodeString(string input)
{

    // create a string exactly as the size needed
    string encoded = String(ENCODED_STRING_LENGTH(input));
    int current = 0;
    // add salt to start of string
    for (string salt = ENCODE_SALT; *salt; encoded[current++] = *(salt++))
        ;

    // encode password by performing not on each char
    for (; *input; encoded[current++] = ~*(input++))
        ;

    // append salt to the end of string too
    for (string salt = ENCODE_SALT; *salt; encoded[current++] = ~*(salt++))
        ;
    encoded[current] = '\0';
    return encoded;
}

// TODO: to be implemented
void updateAppConfig(AppConfig *config)
{
    // this method helps the app to auto login if user has been logged in before
}