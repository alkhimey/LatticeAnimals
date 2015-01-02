#ifndef __SIMPLEHTTP_H
#define __SIMPLEHTTP_H

#define BUFFER_MAX_SIZE 100000

/* Performs simple http get. Returns the complete response including headers */
char* httpGet(const char* host, const char* page, int portno);

/* Extract the content part out of an http response. Will return pointer to the beginning of
   the content or NULL if response does not contain it.
*/
char* extractContent(char* response);

#endif















