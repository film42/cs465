import java.math.BigInteger;
import java.security.SecureRandom;
import java.util.Random;

/**
 * Created by: film42 on: 10/10/14.
 */
public class RSA {

  public static void main(String[] args) {
    Random rnd = new SecureRandom();

//    BigInteger p = BigInteger.probablePrime(512, rnd);
//    BigInteger q = BigInteger.probablePrime(512, rnd);

    BigInteger p = new BigInteger("6997040548236306032270028142697844577679802860009390483825615834396766817911865846408328563633463004219619254639348141491179371158391819359706834377936017");
    BigInteger q = new BigInteger("8593701979078313474945043728666349994614714557320430419781846880808258697302068260916571720507008823663836468130075453764283766701743551377885890781118909");
    BigInteger phiN = p.subtract(BigInteger.valueOf(1)).multiply(q.subtract(BigInteger.valueOf(1)));
    BigInteger n = p.multiply(q);
    BigInteger e = BigInteger.valueOf(65537);
    BigInteger d = Utils.gcd(phiN, e)[2];

    // Encrypt
    BigInteger messageToEncrypt = new BigInteger("212788291171290661843158364587637197333531097895143818988755059744299426205080018037256906030614828346200464605058471107535343001238694433356829682872");

    BigInteger encrypted = Utils.modularExponentiation( messageToEncrypt, e, n );

    BigInteger messageToDecrypt = new BigInteger("38836004958973333808444474744049950597152340928568277009922464771958929445481001650323472151575854567252425265205811145901606099326089029141609035935574695159638546746822086067633268843681771212436983924976927634925742602911873891599802616821582340073810951502143926279012766864221984005313249745229167595735");

    BigInteger decrypted = Utils.modularExponentiation( messageToDecrypt, d, n );

    System.out.println("P: " + p.toString() );
    System.out.println("Q: " + q.toString());
    System.out.println("Phi-N: " + phiN.toString() );
    System.out.println("Prime given e?: " + Utils.gcd(phiN, e)[0] );
    System.out.println("N: " + n.toString());
    System.out.println("E: " + e.toString());
    System.out.println("D: " + d.toString());
    System.out.println("Encrypted: " + encrypted.toString());
    System.out.println("Decrypted: " + decrypted.toString());


  }

}
