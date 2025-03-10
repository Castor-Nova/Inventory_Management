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

int main() {
    displayProducts();

    purchaseProduct(2, 10);
    purchaseProduct(3, 5);

    displayProducts();
}