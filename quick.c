#include "omnomnum.h"
#include <stdio.h>
#include <string.h>
int main(int argc, char** argv){
  if(argc<2){ fprintf(stderr,"usage: quick <string>\n"); return 1; }
  initOmNomNum();
  ParserState st; initParserState(&st); st.parse_second=true; st.precision=3;
  const char* s = argv[1];
  normalize(s, strlen(s), &st);
  printf("%s\n", st.result);
  sdsfree(st.result); freeParserState(&st); freeOmNomNum();
  return 0;
}
