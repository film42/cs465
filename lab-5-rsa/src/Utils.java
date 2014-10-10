import java.math.BigInteger;

/**
 * Created by: film42 on: 10/10/14.
 */
public class Utils {

  // Port from my C# for DH lab
  static BigInteger modularExponentiation(BigInteger base, BigInteger exponent, BigInteger modulo) {

    // Base case
    if( exponent.equals( BigInteger.ZERO ) ) return BigInteger.ONE;

    BigInteger z = modularExponentiation(base, exponent.divide(BigInteger.valueOf(2)), modulo);

    // Even
    if( exponent.getLowestSetBit() != 0 ) {
      return z.pow(2).mod( modulo );
    } else {
      return z.pow(2).multiply( base ).mod( modulo );

    }
  }

  // Based on pseudo code found here: http://introcs.cs.princeton.edu/java/78crypto/
  // His implementation used an array, so that's nice
  static BigInteger[] gcd(BigInteger p, BigInteger q) {
    if (q.equals( BigInteger.ZERO ))
      return new BigInteger[] { p, BigInteger.ONE, BigInteger.ZERO };

    BigInteger[] values = gcd(q, p.mod(q) );
    BigInteger d = values[0];
    BigInteger a = values[2];
    BigInteger b = values[1].subtract( p.divide(q).multiply( values[2] ) );
    return new BigInteger[] { d, a, b };
  }
}
