package project;

import ecomm.Globals;
import ecomm.Product;
import ecomm.Globals.Category;

public class Book extends Product {

    String name, productID;                            //variables to store name,productID
    float price;                                       //price and quantity.
    int quantity;

    @Override
	public Globals.Category getCategory(){             //method to get category of this product.
        return Category.Book;   
    }

    @Override
	public String getName(){                           //method to get name of this product.
        return name;
    }
    
    @Override
	public String getProductID(){                      //method to get productID of this product.
        return productID;
    }

    @Override
	public float getPrice(){                           //method to get price of this product.  
        return price;
    }

    @Override
	public int getQuantity(){                          //method to get quantity of this product.
        return quantity;
    }
    
    public void setName(String name) {                 //method to set the name of this product.
        this.name = name;
    }
    
    public void setProductID(String productID){        //method to set the productID of the product.  
        this.productID = productID;
    }
    
    public void setPrice(float price){                 //method to set the price of the product. 
        this.price = price;
    }
    
    public void setQuantity(int quantity){             //method to set the quantity of the product.
        this.quantity = quantity;
    }
    
}
