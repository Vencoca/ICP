//Standard C++ 11 time measure, platform independent
#include <chrono>
int main(int argc, char * argv[])
{
  auto start = std::chrono::steady_clock::now();

  my_function();
  
  auto end = std::chrono::steady_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cout << "elapsed time: " << elapsed_seconds.count() << "sec" << std::endl;
}


//Not so precise time measure, only in Windows:
int main(int argc, char * argv[])
{
	unsigned long int i,j;
   
	i = GetTickCount();

  my_function();

	j = GetTickCount();

	printf("%u miliseconds\n",j-i);
}


//Precise time measure, only in windows
int main(int argc, char* argv[])
{
    __int64 freq,start,stop;

    QueryPerformanceFrequency( ( LARGE_INTEGER * ) & freq );
    QueryPerformanceCounter( ( LARGE_INTEGER * ) & start );

    my_function();
    
    QueryPerformanceCounter( ( LARGE_INTEGER * ) & stop );

    printf( "%lf sec\n", (double)( ( stop - start ) / ( double )freq ) );

    return 0;
}
  
   