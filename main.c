#include <stdio.h>

typedef struct {
    int id;
    char name[50];
    float price;
    int quantity;
} Product;

void displayProducts() {
    FILE *file = fopen("products.txt", "r");
    Product p;

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("ID\tName\tPrice\tQuantity\n");
    while (fscanf(file, "%d %49s %f %d", &p.id, p.name, &p.price, &p.quantity) != EOF) {
        printf("%d\t%s\t%.2f\t%d\n", p.id, p.name, p.price, p.quantity);
    }

    fclose(file);
}

void purchaseProduct(int id, int quantity) {
    FILE *file = fopen("products.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    Product p;
    int found = 0;

    if (file == NULL || tempFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fscanf(file, "%d %49s %f %d", &p.id, p.name, &p.price, &p.quantity) != EOF) {
        if (p.id == id) {
            found = 1;
            if (p.quantity >= quantity) {
                p.quantity -= quantity;
                printf("Purchase successful!\n");
            } else {
                printf("Insufficient stock.\n");
            }
        }
        fprintf(tempFile, "%d %s %.2f %d\n", p.id, p.name, p.price, p.quantity);
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("products.txt");
        rename("temp.txt", "products.txt");
    } else {
        printf("Product not found.\n");
    }
}

void addProduct(Product newProduct) {
    FILE *file = fopen("products.txt", "a");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "%d %s %.2f %d\n", newProduct.id, newProduct.name, newProduct.price, newProduct.quantity);
    printf("Product added successfully!\n");

    fclose(file);
}

void deleteProduct(int id) {
    FILE *file = fopen("products.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    Product p;
    int found = 0;

    if (file == NULL || tempFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fscanf(file, "%d %49s %f %d", &p.id, p.name, &p.price, &p.quantity) != EOF) {
        if (p.id != id) {
            fprintf(tempFile, "%d %s %.2f %d\n", p.id, p.name, p.price, p.quantity);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("products.txt");
        rename("temp.txt", "products.txt");
        printf("Product deleted successfully!\n");
    } else {
        printf("Product not found.\n");
    }
}

void logAction(const char *action) {
    char command[256];
    snprintf(command, sizeof(command), "powershell.exe -Command \"& .\\logAction.ps1 '%s'\"", action);
    system(command); // Execute the PowerShell script
}

int main() {
    int choice;
    while (1) {
        // Display menu options
        printf("\nOptions:\n");
        printf("1: Display Products\n");
        printf("2: Purchase Products\n");
        printf("3: Add Products\n");
        printf("4: Delete Products\n");
        printf("0: Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 0) {
            printf("Exiting the program.\n");
            break;
        }

        switch (choice) {
            case 1: // Display products
                displayProducts();
                logAction("Displayed all products.");
                break;

            case 2: { // Purchase products
                int id, quantity;
                printf("Enter Product ID to purchase: ");
                scanf("%d", &id);
                printf("Enter quantity to purchase: ");
                scanf("%d", &quantity);
                purchaseProduct(id, quantity);
                
                char log[100];
                snprintf(log, sizeof(log), "Purchased %d of product ID %d.", quantity, id);
                logAction(log);
                break;
            }

            case 3: { // Add product
                Product newProduct;
                printf("Enter Product ID: ");
                scanf("%d", &newProduct.id);
                printf("Enter Product Name: ");
                scanf("%s", newProduct.name);
                printf("Enter Product Price: ");
                scanf("%f", &newProduct.price);
                printf("Enter Product Quantity: ");
                scanf("%d", &newProduct.quantity);
                addProduct(newProduct);
                
                char log[100];
                snprintf(log, sizeof(log), "Added product ID %d (%s).", newProduct.id, newProduct.name);
                logAction(log);
                break;
            }

            case 4: { // Delete product
                int id;
                printf("Enter Product ID to delete: ");
                scanf("%d", &id);
                deleteProduct(id);
                
                char log[100];
                snprintf(log, sizeof(log), "Deleted product ID %d.", id);
                logAction(log);
                break;
            }

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
