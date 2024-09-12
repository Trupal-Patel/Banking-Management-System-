import java.util.Scanner;
import project.Platform102;
import project.Seller_070;
import project.Seller_102;
import project.Seller_056;
import ecomm.Globals;
import ecomm.Platform;
import ecomm.Seller;

import java.io.FileWriter; // Import the FileWriter class
import java.io.IOException; // Import the IOException class to handle errors

public class PlatformMain {

	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);
		Platform pf = new Platform102(); // replace with appropriate derived class

		// create a number of sellers (of different sub-types of Seller)
		// Assign a name (sellerID) to each of them.

		Seller s1 = new Seller_070("seller070");
		s1.addPlatform(pf);

		Seller s2 = new Seller_102("seller102");
		s2.addPlatform(pf);

		Seller s3 = new Seller_056("seller056");
		s3.addPlatform(pf);
		
		// Below piece of code to flush the PlatformToPortal.txt file
		try{
			FileWriter fw = new FileWriter(Globals.fromPlatform);
			fw.close();
		}
		catch (IOException e) {
			System.out.println("An error occurred.\n");
			e.printStackTrace();
		}

		// Processes requests from the PortalToPlatform.txt file when the input is "Check".
		// terminates the program when input is "End"
		while (true) {
			String s = sc.next();
			if(s.equals("Check"))
			{
				pf.processRequests();
			}
			else if(s.equals("End"))
			{
				break;
			}
		}
		
		// Below piece of code to flush the PlatformToPortal.txt file
		try{
			FileWriter fw = new FileWriter(Globals.fromPlatform);
			fw.close();
		}
		catch (IOException e) {
			System.out.println("An error occurred.\n");
			e.printStackTrace();
		}
		sc.close();
	}

}
