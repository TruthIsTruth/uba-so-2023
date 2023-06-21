#include <linux/init.h>
#include <linux/module.h> // THIS_MODULE
#include <linux/kernel.h> // Kernel cosas
#include <linux/fs.h>     // File operations
#include <linux/cdev.h>   // Char devices
#include <linux/device.h> // Nodos filesystem
#include <linux/uaccess.h> // copy_to_user
#include <linux/slab.h>    // kmalloc
#include <linux/random.h>  // get_random_bytes

#define DEVICE_NAME "azar"

struct cdev azar_dev;
static struct class *mi_class;
static dev_t major;

static int numero;

static ssize_t azar_read(struct file *filp, char __user *data, size_t s, loff_t *off) {
	unsigned int rand = 0;
    get_random_bytes(&rand, sizeof(rand));
    rand = rand % numero;

    char *buffer = kmalloc(s+1, GFP_KERNEL);
    int leidos = snprintf(buffer, s, "%u", rand);
    if(leidos <= 0){
        kfree(buffer);
        return -EPERM;
    }
    buffer[s] = '\n';

    copy_to_user(data, buffer, leidos+1);
    kfree(buffer);
    
    return s;
}

static ssize_t azar_write(struct file *filp, const char __user *data, size_t s, loff_t *off) {
	char *buffer = kmalloc(s+1, GFP_KERNEL);
    copy_from_user(buffer, data, s);
    
    buffer[s] = '\0';

    if (0 == kstrtoint(buffer, 10, &numero)){
        kfree(buffer);
        return s;
    }
    kfree(buffer);
    return -EPERM;
}

static struct file_operations azar_operaciones = {
	// Completar
	.owner = THIS_MODULE,
	.read = azar_read,
	.write = azar_write
};

static int __init azar_init(void) {
	//Conseguir los device numbers (el major y el minor)
	alloc_chrdev_region(&major, 0, 1, "azar_dev"); //firstminor = 0; count = 1
	
    //Inicializar el device como un char device
	cdev_init(&azar_dev, &azar_operaciones);	
	//Asignar los números al dispositivo que inicializamos previamente
	cdev_add(&azar_dev, major, 1); //count = 1
	
	//Crear los nodos correspondientes en el sistema de archivos.
	mi_class = class_create(THIS_MODULE, DEVICE_NAME);
	device_create(mi_class, NULL, major, NULL, DEVICE_NAME);
	return 0;
}

static void __exit azar_exit(void) {
    //Destruir nodos - Primero
	device_destroy(mi_class, major);
	class_destroy(mi_class);
	//Liberar recursos - Segundo
	unregister_chrdev_region(major, 1); //count = 1
	cdev_del(&azar_dev);
}

// Completar
module_init(azar_init);
module_exit(azar_exit);

// Completar

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Generador de números al azar");