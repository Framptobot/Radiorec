bool readSignal( int botnum )
{
  long now = micros();
  if( 0 == digitalRead(RCPIN) )
  {
    return( false );
  }

  long zstart;
  long p1;
  do
  {
    zstart = micros();
    p1 = zstart-now;
  } while( ( p1 < (PER*5) ) && ( 1 == digitalRead(RCPIN) ) );
  if( (p1<(PER*3)) || (p1>=(PER*5)) )
  {
    return( false );
  }

  char tbuf[5];
  char* ptr = tbuf;
  int ended = 0;
  byte sum = 0;
  long bitstart;
  do
  {
    bitstart = micros();
    p1 = bitstart-zstart;
  } while( (p1<(PER*3)) && ( 0 == digitalRead(RCPIN) ) );
  if( p1 >= (PER*3) )
  {
    return( false );
  }
  for( int b=0; (b<5)&&!ended; b++ )
  {
    unsigned char c = 0;
    for( int i=0; i<8; i++ )
    {
      int expBit = (i&1)^1;
      long bitend;
      long dt;
      do
      {
        bitend = micros();
        dt = bitend-bitstart;
      } while( (dt<(PER*3)) && ( expBit == digitalRead(RCPIN) ) );
      if( dt >= (PER*3) )
      {
        return( false );
      }
      c>>=1;
      if( dt>(PER+(PER/2)) )
      {
        c |= 128;
      }
      bitstart = bitend;
    }
    *ptr++ = c;
    sum += c;
  }
  
  if( sum != 0xff )
  {
    return( false );
  }

  if( tbuf[0] != botnum )
  {
    return( false );
  }

  
  for( unsigned int i=0; i<5; i++ )
  {
    inputBuf[i] = tbuf[i];
  }
  return( true );
}
