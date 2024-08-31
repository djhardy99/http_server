int summ(int& a, int& b){
  int * p = &a;
  int c = *p;
  p = &b;
  c += *p;
  return c;
}