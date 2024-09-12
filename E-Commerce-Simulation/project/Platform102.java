
package project;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;
import java.io.BufferedWriter;
import java.io.File; // Import the File class
import java.io.FileWriter; // Import the FileWriter class
import java.io.IOException; // Import the IOException class to handle errors

import ecomm.Globals;
import ecomm.Platform;
import ecomm.Product;
import ecomm.Seller;

public class Platform102 extends Platform {

	ArrayList<Seller> mysellers = new ArrayList<>();	// Arraylist of all sellers attached to the platform.
	int lastResponseID = 0;	// To process only the requests that are not yet processed.
	Boolean flag = false;	// flag to distinguish the first request, since the attribute lastResponseID is undefined for that.

	@Override
	// Method to attach seller to the platform.
	public boolean addSeller(Seller aSeller) {
		mysellers.add(aSeller);
		return true;
	}

	@Override
	// processes the requests read from the PortalToPlatform.txt file.
	public void processRequests() {
		try {
			File myObj = new File(Globals.toPlatform);	// File Object used to read the PortalToPlatform.txt file.
			Scanner myReader = new Scanner(myObj);

			Globals globalobj = new Globals();	// Globals object to use its getCategoryName method to write in the PlatformToPortal.txt file.

			// This HashMap does the exact reverse job of Globals.getCategoryName method.
			// For example It converts "Mobile" to Globals.Category.Mobile
			HashMap<String, Globals.Category> mapStringToCategory = new HashMap<>();
			mapStringToCategory.put("Book", Globals.Category.Book);
			mapStringToCategory.put("Mobile", Globals.Category.Mobile);

			// BufferedWriter used to append to the file instead of creating an empty file.
			try(FileWriter fw = new FileWriter(Globals.fromPlatform, true);
			BufferedWriter myWriter = new BufferedWriter(fw);)
			{
				// Reading the PortalToPlatform.txt file line by line to process the requests.
				while (myReader.hasNextLine()) {
					String data = myReader.nextLine();
					String[] inp = data.split(" ");
					String portalID = inp[0];
					int requestID = Integer.parseInt(inp[1]);
					String command = inp[2];

					if(!flag)
					{
						lastResponseID = requestID - 1;
						flag = true;
					}
					
					if(lastResponseID >= requestID) continue;	// To process only the requests that are not yet processed.
					
					// To List all the available categories.
					if (command.equals("Start")) {
						// HashMap to check which categories are available from the given set of Categories in Globals.Category.
						HashMap<Globals.Category, Boolean> categorylist = new HashMap<>();
						for (Globals.Category cat : Globals.Category.values()) {
							categorylist.put(cat, false);	// Mapping all categories to false initially.
						}

						// Traversing through all sellers and checking which categories are present and mapping them to true in categorylist HashMap.
						for (Globals.Category cat : Globals.Category.values()) {
							for (Seller aseller : mysellers) {
								ArrayList<Product> tempproductlist = new ArrayList<>(aseller.findProducts(cat));
								if (tempproductlist.isEmpty())
								continue;
								else {
									categorylist.put(cat, true);
									break;
								}
							}
						}
						// flag to output the portalID and requestID for the first category and not for other categories.
						Boolean flg = false;
						for (HashMap.Entry<Globals.Category, Boolean> set : categorylist.entrySet()) {
							if (set.getValue() && !flg) {
								// Write the response in PlatformToPortal.txt file.
								myWriter.write(portalID + " " + requestID + " " + globalobj.getCategoryName(set.getKey()) + " ");
								flg = true;
							}
							else if(set.getValue())
							{
								// Write the response in PlatformToPortal.txt file.
								myWriter.write(globalobj.getCategoryName(set.getKey()) + " ");
							}
						}
						myWriter.write("\n");
						
					}
					
					// To List all the available products of requested category.
					else if (command.equals("List")) {
						Globals.Category cat = mapStringToCategory.get(inp[3]);
						ArrayList<Product> productlist = new ArrayList<>();
						for (Seller aseller : mysellers) {
							ArrayList<Product> tempproductlist = new ArrayList<>(aseller.findProducts(cat));
							productlist.addAll(tempproductlist);
						}
						
						for(Product product : productlist)
						{
							// Write the response in PlatformToPortal.txt file.
							myWriter.write(portalID + " " + requestID + " " + product.getName() + " " + product.getProductID() + " " + product.getPrice() + " " + product.getQuantity()  + "\n");
						}
						
					}
					
					// To Buy any quantity of a product.
					else if (command.equals("Buy")) {
						String productID = inp[3];
						int quantity = Integer.parseInt(inp[4]);
						String[] productaddress = productID.split("-");	// Since productID is sellerID-productName
						String sellerID = productaddress[0];
						// String productName = productaddress[1];	// Not needed.
						
						// Itereting over all sellers to match the sellerID.
						for (Seller aseller : mysellers) {
							// if the sellerID matches, use the buyProduct method of aseller to check if the request is a success or failure.
							if(aseller.getID().equals(sellerID))
							{
								Boolean success = aseller.buyProduct(productID, quantity);
								if(success)
								{
									// Write the response in PlatformToPortal.txt file.
									myWriter.write(portalID + " " + requestID + " " + "Success\n");
								}
								else
								{
									// Write the response in PlatformToPortal.txt file.
									myWriter.write(portalID + " " + requestID + " " + "Failure\n");
								}
							}
						}
					}
					lastResponseID = requestID;	//setting the lastResponseID as requestID since it has already been processed.
				}
			}
			myReader.close();
		} catch (IOException e) {
			System.out.println("An error occurred.\n");
			e.printStackTrace();
		}
	}
}
