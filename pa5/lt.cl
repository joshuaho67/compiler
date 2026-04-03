-- newbasic.cl
-- create basic classes with 'new', which may
-- mess up assumptions, particularly w/bool

class Main {
  io:IO <- new IO;
  x:Int;

  main():Object {{
  
  while ( {io.out_int(x);x<5;}) loop {
  x<-x+1;
  --io.out_int(x);
  } pool;


  }};
};

