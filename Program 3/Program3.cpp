#include<math.h>
#include<string.h>
#include<omp.h>
#include<iostream>
using namespace std;
double t = 0.0;
inline long Strike( bool composite[], long i,long stride, long limit )
{
   for( ; i<=limit; i+=stride )
      composite[i] = true;
   return i;
}

long CacheUnfriendlySieve( long n )
{
   long count = 0;
   long m = (long)sqrt((double)n);
   bool* composite = new bool[n+1];
   memset( composite, 0, n );
   t = omp_get_wtime();
   for( long i=2; i<=m; ++i )
      if( !composite[i] )
      {
         ++count;
         // Strike walks array of size n here.
         Strike( composite, 2*i, i, n );
      }
   for( long i=m+1; i<=n; ++i )
      if( !composite[i] )
         ++count;

   t = omp_get_wtime() - t;

   delete[] composite;

   return count;
}

long CacheFriendlySieve( long n )
{
   long count = 0;
   long m = (long)sqrt((double)n);
   bool* composite = new bool[n+1];
   memset( composite, 0, n );
   long* factor = new long[m];
   long* striker = new long[m];
   long n_factor = 0;

   t= omp_get_wtime();

   for( long i=2; i<=m; ++i )
      if( !composite[i] )
      {
         ++count;
         striker[n_factor] = Strike( composite, 2*i, i, m );
         factor[n_factor++] = i;
      }

   // Chops sieve into windows of size Å sqrt(n)
   for( long window=m+1; window<=n; window+=m )
   {
      long limit = min(window+m-1,n);
      for( long k=0; k<n_factor; ++k )
         // Strike walks window of size sqrt(n) here.
         striker[k] = Strike( composite, striker[k], factor[k],limit );
      for( long i=window; i<=limit; ++i )
         if( !composite[i] )
            ++count;
   }

   t = omp_get_wtime() - t;

   delete[] striker;
   delete[] factor;
   delete[] composite;

   return count;
}

long ParallelSieve( long n, int numThreads)
{
   long count = 0;
   long m = (long)sqrt((double)n);

   long n_factor = 0;
   long* factor = new long[m];

   t= omp_get_wtime();

   omp_set_num_threads(numThreads);
#pragma omp parallel
   {
      bool* composite = new bool[m+1];
      long* striker = new long[m];

#pragma omp single
      {
         memset( composite, 0, m );
         for( long i=2; i<=m; ++i )
            if( !composite[i] )
            {
               ++count;
               striker[n_factor] = Strike( composite, 2*i, i, m );
               factor[n_factor++] = i;
            }
      }
      long base = -1;

      // Chops sieve into windows of size Å sqrt(n)
#pragma omp for reduction(+:count)
      for( long window=m+1; window<=n; window+=m )
      {
         memset(composite, 0, m);
         if(base != window)
         {
            //Must compute stiker from scratch
            base = window;
            for(long k=0; k<n_factor; ++k)
               striker[k] = (base+factor[k]-1)/factor[k] * factor[k] - base;
         }
         
         long limit = min(window+m-1, n) - base;
         for(long k=0; k<n_factor; ++k)
            striker[k] = Strike(composite, striker[k], factor[k], limit) - m;

         for(long i=0; i<=limit; ++i)
            if(!composite[i])
               ++count;
            base += m;
      }

      delete[] striker;
      delete[] composite;
   }

   t = omp_get_wtime() - t;

   delete[] factor;
   return count;
}

int main()
{

   long N;
   cout<<"Enter the value of N: ";
   cin>>N;


   cout<<"CACHE UNFRIENDLY SEIVE"<<endl;
   cout<< "Count = "<<CacheUnfriendlySieve(N)<<endl;
   cout<< "Time = "<<t<<endl;

   cout<<endl;

   cout<<"CACHE FRIENDLY SEIVE"<<endl;
   cout<< "Count = "<<CacheFriendlySieve(N)<<endl;
   cout<< "Time = "<<t<<endl;

   cout<<endl;

   int numThreads;
   cout<<"PARALLEL SEIVE"<<endl;
   cout<<"Enter the number of threads: ";
   cin>>numThreads;
   cout<< "Count = "<<ParallelSieve(N, numThreads)<<endl;
   cout<< "Time = "<<t<<endl;
}
