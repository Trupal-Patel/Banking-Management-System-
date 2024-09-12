package project;

import java.util.ArrayList;

import ecomm.Globals;
import ecomm.Platform;
import ecomm.Product;
import ecomm.Seller;

public class Seller_070 extends Seller {

    ArrayList<Mobile> mobiles = new ArrayList<>();          //Arraylist to store the mobile products.
    ArrayList<Book> books = new ArrayList<>();              //Arraylist to store the books products.

    //constructor which makes the productsand strores into respective arraylist.
    public Seller_070(String id) {
        super(id);
        Mobile m1 = new Mobile();
        m1.setName("m1");
        m1.setPrice(56);
        m1.setProductID(id+"-"+m1.getName());
        m1.setQuantity(4);
        Mobile m2 = new Mobile();
        m2.setName("m2");
        m2.setPrice(57);
        m2.setProductID(id+"-"+m2.getName());               //m1 and m2 are mobile products and b1 and b2 are book products
        m2.setQuantity(5);

        Book b1 = new Book();
        b1.setName("b1");
        b1.setPrice(66);
        b1.setProductID(id+"-"+b1.getName());
        b1.setQuantity(7);
        Book b2 = new Book();
        b2.setName("b2");
        b2.setPrice(67);
        b2.setProductID(id+"-"+b2.getName());
        b2.setQuantity(8);

        //adding into arraylist.
        mobiles.add(m1);
        mobiles.add(m2);
        books.add(b1);
        books.add(b2);
    }

    @Override
    //method to add the seller to platform.
    public void addPlatform(Platform thePlatform) {
        thePlatform.addSeller(this);
    }

    @Override
    // method to return the arraylist which contains all the products of the given category.
    public ArrayList<Product> findProducts(Globals.Category whichOne) {
        ArrayList<Product> a = new ArrayList<>();
        //we traverse all the books if the given category is book.
        if (whichOne == Globals.Category.Book) {
            for (Book i : books) {
                if (i.getCategory().equals(whichOne) && i.getQuantity() != 0) {
                    a.add(i);
                }
            }
        }
        //we traverse all the mobile if the given category is mobile.
        else if (whichOne == Globals.Category.Mobile) {
            for (Mobile i : mobiles) {
                if (i.getCategory().equals(whichOne) && i.getQuantity() != 0) {
                    a.add(i);
                }
            }
        }
        return a;
    }

    @Override
    //method which handles the buying process of the products.
    public boolean buyProduct(String productID, int quantity) {

        //First we traverse the mobile list and if any product matches with the given productID 
        // then we check the quantity of that product if the quantity suffices then the boolean returns true otherwise false.
        for (Mobile i : mobiles) {
            if (i.getProductID().equals(productID)) {
                if (i.getQuantity() >= quantity) {
                    i.setQuantity(i.getQuantity() - quantity);
                    return true;
                } else {
                    return false;
                }
            }
        }

        //after that we iterate through all the books and check the same as before.
        for (Book i : books) {
            if (i.getProductID().equals(productID)) {
                if (i.getQuantity() >= quantity) {
                    i.setQuantity(i.getQuantity() - quantity);
                    return true;
                } else {
                    return false;                       // if we don't get any product which mathches with the given ProductID then we return false
                }
            }
        }

        return false;
    }
}
