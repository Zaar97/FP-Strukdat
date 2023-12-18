#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Item {
public:
    virtual void display() const = 0;
    virtual ~Item() {}
};

class Product : public Item {
private:
    std::string name;
    double price;

public:
    Product(const std::string& n, double p) : name(n), price(p) {}

    void display() const override {
        std::cout << "Product: " << name << ", Price: $" << price << std::endl;
    }

    // Getter for product name
    std::string getName() const {
        return name;
    }

    // Setter for product price
    void setPrice(double newPrice) {
        price = newPrice;
    }
};

class StoreInventory {
private:
    std::vector<Item*> items;
    std::vector<std::vector<bool>> adjacencyMatrix;
    std::vector<std::string> bundleNames;

public:
    void addItem(Item* item) {
        items.push_back(item);
        adjacencyMatrix.push_back(std::vector<bool>(items.size(), false));
    }

    int findIndexByName(const std::string& itemName) const {
        for (int i = 0; i < items.size(); ++i) {
            if (const Product* product = dynamic_cast<const Product*>(items[i])) {
                if (product->getName() == itemName) {
                    return i;
                }
            }
        }
        return -1;
    }

    int findBundleIndex(const std::string& bundleName) const {
        auto it = std::find(bundleNames.begin(), bundleNames.end(), bundleName);
        if (it != bundleNames.end()) {
            return std::distance(bundleNames.begin(), it);
        }
        return -1;
    }

    void addBundleAndItems(const std::string& bundleName, const std::string& productName1, const std::string& productName2) {
        int bundleIndex = findBundleIndex(bundleName);
        int productIndex1 = findIndexByName(productName1);
        int productIndex2 = findIndexByName(productName2);

        if (bundleIndex == -1) {
            bundleNames.push_back(bundleName);
            bundleIndex = bundleNames.size() - 1;
        }

        if (productIndex1 != -1 && productIndex2 != -1) {
            adjacencyMatrix[bundleIndex][productIndex1] = true;
            adjacencyMatrix[bundleIndex][productIndex2] = true;
            std::cout << "Bundle and items added successfully.\n";
        } else {
            std::cout << "One or both products not found.\n";
        }
    }

    void displayBundleItems(const std::string& bundleName) const {
        int bundleIndex = findBundleIndex(bundleName);
        if (bundleIndex != -1) {
            bool foundBundle = false;
            std::cout << "Products bundled in " << bundleName << ":\n";
            for (int j = 0; j < adjacencyMatrix.size(); j++) {
                if (adjacencyMatrix[bundleIndex][j]) {
                    items[j]->display();
                    foundBundle = true;
                }
            }
            if (!foundBundle) {
                std::cout << "No bundled products found for " << bundleName << ".\n";
            }
        } else {
            std::cout << "Bundle not found.\n";
        }
    }

    void displayInventory() const {
        std::cout << "Store Inventory:\n";
        for (const auto& item : items) {
            item->display();
        }
    }

    Item* findItem(const std::string& itemName) const {
        for (const auto& item : items) {
            if (const Product* product = dynamic_cast<const Product*>(item)) {
                if (product->getName() == itemName) {
                    return item;
                }
            }
        }
        return nullptr;
    }

    void updateProduct(const std::string& productName, double newPrice) {
        Item* item = findItem(productName);
        if (item) {
            dynamic_cast<Product*>(item)->setPrice(newPrice);
            std::cout << "Product updated successfully.\n";
        } else {
            std::cout << "Product not found.\n";
        }
    }

    void deleteProduct(const std::string& productName) {
        auto it = std::remove_if(items.begin(), items.end(), [&](Item* item) {
            return dynamic_cast<Product*>(item) && dynamic_cast<Product*>(item)->getName() == productName;
        });

        if (it != items.end()) {
            delete *it;
            items.erase(it, items.end());
            std::cout << "Product deleted successfully.\n";
        } else {
            std::cout << "Product not found.\n";
        }
    }

    ~StoreInventory() {
        for (const auto& item : items) {
            delete item;
        }
    }
};

class Store {
private:
    StoreInventory inventory;

public:
    void addItem(Item* item) {
        inventory.addItem(item);
    }

    void displayStoreInventory() const {
        inventory.displayInventory();
    }

    void addProduct(const std::string& productName, double price) {
        Product* newProduct = new Product(productName, price);
        inventory.addItem(newProduct);
        std::cout << "Product added successfully.\n";
    }

    void updateProduct(const std::string& productName, double newPrice) {
        inventory.updateProduct(productName, newPrice);
    }

    void deleteProduct(const std::string& productName) {
        inventory.deleteProduct(productName);
    }

    void addBundleAndItems(const std::string& bundleName, const std::string& productName1, const std::string& productName2) {
        inventory.addBundleAndItems(bundleName, productName1, productName2);
    }

    void displayBundleItems(const std::string& bundleName) const {
        inventory.displayBundleItems(bundleName);
    }
};

int main() {
    Store myStore;

    int choice;
    std::string productName, productName1, productName2, bundleName;
    double price;

    do {
        std::cout << "\nOptions:\n"
                     "1. Add Product\n"
                     "2. Update Product\n"
                     "3. Delete Product\n"
                     "4. Add Bundle and Items\n"
                     "5. Display Bundle Items\n"
                     "6. Display Products\n"
                     "7. Exit\n"
                     "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Enter product name: ";
                std::cin >> productName;
                std::cout << "Enter product price: $ ";
                std::cin >> price;
                myStore.addProduct(productName, price);
                break;

            case 2:
                std::cout << "Enter product name to update: ";
                std::cin >> productName;
                std::cout << "Enter new price: ";
                std::cin >> price;
                myStore.updateProduct(productName, price);
                break;

            case 3:
                std::cout << "Enter product name to delete: ";
                std::cin >> productName;
                myStore.deleteProduct(productName);
                break;

            case 4:
                std::cout << "Enter bundle name: ";
                std::cin >> bundleName;
                std::cout << "Enter product name 1: ";
                std::cin >> productName1;
                std::cout << "Enter product name 2: ";
                std::cin >> productName2;
                myStore.addBundleAndItems(bundleName, productName1, productName2);
                break;

            case 5:
                std::cout << "Enter bundle name: ";
                std::cin >> bundleName;
                myStore.displayBundleItems(bundleName);
                break;

            case 6:
                myStore.displayStoreInventory();
                break;

            case 7:
                std::cout << "Exiting...\n";
                break;

            default:
                std::cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 7);

    return 0;
}
