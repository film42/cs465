// MOD EXP

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Numerics;

namespace ConsoleApplication2 {
  class ModExp {

    // Class instance of random
    private static readonly Random random = new Random();

    public BigInteger modularExponentiation(BigInteger b, BigInteger power, BigInteger num) {
      // Recursive base case, we don't want a multiply by 0
      if (power == 0) return 1;

      // Recursive call and Shift the power to the right
      BigInteger z = modularExponentiation(b, BigInteger.Divide(power, 2), num);
      // Check to see if the bit is odd or even
      // Then, perform multiplication followed by modular reduction
      if (power.IsEven) return BigInteger.Pow(z, 2) % num;
      else {
        // Odd bit, so we multiply it by "b"
        BigInteger z_squared = BigInteger.Pow(z, 2);
        return BigInteger.Multiply(z_squared, b) % num;
      }
    }

    public BigInteger uniformNumber(BigInteger max) {
      // Get the bit count of a number (rounding up)
      double bit_count = BigInteger.Log(max, 2);
      int length = (int)Math.Ceiling(bit_count);

      // Grab some random int from the persisted random instance
      int random_int = random.Next();

      // Generate some BigInt with at least bit_count above
      BigInteger random_number = 0;
      for (int i = 0; i <= length / 32; i++)
        random_number = (random_number << 32) + random.Next();

      // Ensure that number is between 1 and max-1
      return random_number % (max - 1) + 1;
    }

  }
}


// PROGRAM

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Numerics;
using System.Globalization;

namespace ConsoleApplication2 {
  class Program {

    public static BigInteger GeneratePrime(string s) {
      return BigInteger.Parse(s, NumberStyles.AllowHexSpecifier);
    }

    static void Main(string[] args) {

      var our_p = "7f1830d98a876f67465b3a091fe30820b6421b2f3250347823f96b4531a4c17c2098bb1d06a74a796adb531e1e6acfa971e06a7cfedf6f68fc910c2a2dd316";
      var their_p_str = "21432403557162374932441289038736814310790299493161805908820528195512735486334846335096883937910361779580353435606306852844838379875775209207869981185455";
      var their_p = BigInteger.Parse(their_p_str);

      ModExp logic = new ModExp();

      var p = GeneratePrime(our_p);
      int g = 5;
      int s = 17;

      var pub = logic.modularExponentiation( g, s, p );

      var common = logic.modularExponentiation( their_p, s, p );


      Console.WriteLine( common );

      Console.ReadKey();
    }
  }
}
