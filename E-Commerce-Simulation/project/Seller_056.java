package project;
import java.util.ArrayList;
import ecomm.Globals;
import ecomm.Platform;
import ecomm.Product;
import ecomm.Seller;

public class Seller_056 extends Seller {                    

    ArrayList<Mobile> mobiles = new ArrayList<>();          //Arraylist to store the mobile products.
    ArrayList<Book> books = new ArrayList<>();              //Arraylist to store the books products.

    public Seller_056(String id) {                        //constructor which makes the products
        super(id);                                          //and strores into respective arraylist.
        Mobile m1 = new Mobile();
        m1.setName("m1");
        m1.setPrice(111);
        m1.setProductID(id+"-"+m1.getName());
        m1.setQuantity(11);

        Mobile m2 = new Mobile();                           //m1 and m2 are mobile products and b1 and b2 are book 
        m2.setName("m2");                             //products. 
        m2.setPrice(112);
        m2.setProductID(id+"-"+m2.getName());
        m2.setQuantity(12);

        Mobile m3 = new Mobile();                           
        m3.setName("m3");                             
        m3.setPrice(113);
        m3.setProductID(id+"-"+m3.getName());
        m3.setQuantity(13);

        Book b1 = new Book();
        b1.setName("b1");
        b1.setPrice(211);
        b1.setProductID(id+"-"+b1.getName());
        b1.setQuantity(21);

        Book b2 = new Book();
        b2.setName("b2");
        b2.setPrice(212);
        b2.setProductID(id+"-"+b2.getName());
        b2.setQuantity(22);

        Book b3 = new Book();
        b3.setName("b3");
        b3.setPrice(213);
        b3.setProductID(id+"-"+b3.getName());
        b3.setQuantity(23);

        mobiles.add(m1);mobiles.add(m2);mobiles.add(m3);                    //adding into arraylist.
        books.add(b1);books.add(b2);books.add(b3);
    }

    @Override
    public void addPlatform(Platform thePlatform) {         //method to add the seller to platform.
        thePlatform.addSeller(this);
    }

    @Override
    public ArrayList<Product> findProducts(Globals.Category whichOne) {             //method to return the arraylist 
        ArrayList<Product> a = new ArrayList<>();                                   //which contains all the products of the given      
        if (whichOne == Globals.Category.Book) {                                    //category.
            for (Book i : books) {
                if (i.getCategory().equals(whichOne) && i.getQuantity() != 0){      //we traverse all the books if the given category is book.
                    a.add(i);
                }
            }
        } else if (whichOne == Globals.Category.Mobile) {
            for (Mobile i : mobiles) {
                if (i.getCategory().equals(whichOne) && i.getQuantity() != 0){      //we traverse all the mobile if the given category is mobile.
                    a.add(i);
                }
            }
        }
        return a;
    }

    @Override
    public boolean buyProduct(String productID, int quantity) {                    //method which handles the buying process of the products.     

        for (Mobile i : mobiles) {                                                 //First we traverse the mobile list and if  
            if (i.getProductID().equals(productID)) {                              //any product matches with the given productID then we check  
                if (i.getQuantity() >= quantity) {                                 //the quantity of that product if the quantity suffices then      
                    i.setQuantity(i.getQuantity() - quantity);                     //the boolean returns true otherwise false 
                    return true;
                } else {
                    return false;
                }
            }
        }

        for (Book i : books) {                                                      //after that we iterate through all the books and check the same as before.
            if (i.getProductID().equals(productID)) {
                if (i.getQuantity() >= quantity) {
                    i.setQuantity(i.getQuantity() - quantity);
                    return true;
                } else {
                    return false;                                                   // if we don't get any product which mathches with the given ProductID
                }                                                                   // then we return false  
            }
        }

        return false;
    }
}
