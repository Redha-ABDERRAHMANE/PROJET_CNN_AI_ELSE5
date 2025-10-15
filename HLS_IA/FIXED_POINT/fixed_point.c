#include "fixed_point.h"

// Position de la virgule (nombre de bits fractionnaires)
// Modifiable selon vos besoins
static uint8_t FRAC_BITS = 16; // Q16.16 par défaut

// Conversion float vers fixed-point (int32_t)
int32_t float_to_fixed(float f) {
    return (int32_t)(f * (1 << FRAC_BITS));
}

// Conversion fixed-point vers float
float fixed_to_float(int32_t fixed) {
    return (float)fixed / (float)(1 << FRAC_BITS);
}

// Changer la position de la virgule pour une valeur
int32_t change_fixed_format(int32_t fixed, uint8_t old_frac_bits, uint8_t new_frac_bits) {
    int shift = new_frac_bits - old_frac_bits;
    
    if (shift > 0) {
        return fixed << shift;  // Plus de précision
    } else {
        return fixed >> (-shift); // Moins de précision
    }
}

// Définir globalement la position de la virgule
void set_fractional_bits(uint8_t frac_bits) {
    FRAC_BITS = frac_bits;
}

// Addition
int32_t fixed_add(int32_t a, int32_t b) {
    return a + b;
}

// Soustraction
int32_t fixed_sub(int32_t a, int32_t b) {
    return a - b;
}

// Multiplication
int32_t fixed_mul(int32_t a, int32_t b) {
    int64_t temp = (int64_t)a * (int64_t)b;
    return (int32_t)(temp >> FRAC_BITS);
}

// Division
int32_t fixed_div(int32_t a, int32_t b) {
    int64_t temp = ((int64_t)a << FRAC_BITS);
    return (int32_t)(temp / b);
}

// Affichage
void print_fixed(int32_t fixed) {
    printf("0x%08X = %f (Q%d.%d)\n", 
           fixed, 
           fixed_to_float(fixed),
           32 - FRAC_BITS,
           FRAC_BITS);
}

int main() {
    printf("=== Fixed-Point sur int32_t ===\n\n");
    
    // Test avec Q16.16
    printf("Format Q16.16 (virgule au 16ème bit):\n");
    set_fractional_bits(16);
    
    float val = 123.456f;
    int32_t fixed = float_to_fixed(val);
    
    printf("Float: %f\n", val);
    print_fixed(fixed);
    
    // Conversion vers Q8.24
    printf("\nConversion vers Q8.24:\n");
    int32_t fixed_24 = change_fixed_format(fixed, 16, 24);
    set_fractional_bits(24);
    print_fixed(fixed_24);
    
    // Conversion vers Q24.8
    printf("\nConversion vers Q24.8:\n");
    int32_t fixed_8 = change_fixed_format(fixed, 16, 8);
    set_fractional_bits(8);
    print_fixed(fixed_8);
    
    // Opérations
    printf("\n=== Opérations ===\n");
    set_fractional_bits(16);
    
    int32_t a = float_to_fixed(10.5f);
    int32_t b = float_to_fixed(2.25f);
    
    printf("a = %f, b = %f\n", fixed_to_float(a), fixed_to_float(b));
    
    int32_t sum = fixed_add(a, b);
    printf("a + b = %f\n", fixed_to_float(sum));
    
    int32_t prod = fixed_mul(a, b);
    printf("a * b = %f\n", fixed_to_float(prod));
    
    int32_t quot = fixed_div(a, b);
    printf("a / b = %f\n", fixed_to_float(quot));
    
    // Représentation binaire
    printf("\n=== Représentation binaire ===\n");
    int32_t test = float_to_fixed(5.75f); // 5.75 = 5 + 0.75 = 5 + 3/4
    printf("Valeur: 5.75\n");
    printf("Binaire: ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (test >> i) & 1);
        if (i == 16) printf(" | "); // Séparation virgule
    }
    printf("\n         ");
    printf("Partie entière (16 bits) | Partie fractionnaire (16 bits)\n");
    print_fixed(test);
    
    return 0;
}