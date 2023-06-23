#include <linux/init.h>
#include <linux/module.h> // THIS_MODULE
#include <linux/kernel.h> // Kernel cosas
#include <linux/fs.h>     // File operations
#include <linux/cdev.h>   // Char devices
#include <linux/device.h> // Nodos filesystem
#include <linux/uaccess.h> // copy_to_user
#include <linux/slab.h>    // kmalloc
#include <linux/random.h>  // get_random_bytes

#define DEVICE_NAME "letras123"

struct cdev letras123_dev;
static struct class *mi_class;
static dev_t major;

spinlock_t lock;
char espacios[3];
bool libres[3];
typedef struct espacio {
    int index;
    bool wrote;
} espacio_t;


static int letras123_open(struct inode *inod, struct file *filp) {
    int index = -1;
    int i;
    bool found;
    espacio_t *userdata;

    spin_lock(&lock);
        i = 0;
        found = false;
        while (i < 3 || found) {
            if (libres[i]) {
                index = i;
                libres[i] = false;
                found = true;
            }
            i++;
        }
    spin_unlock(&lock);

    if (index == -1) {
        return -EPERM;
    } else {
        userdata = (espacio_t *) kmalloc(sizeof(espacio_t), GFP_KERNEL);
        userdata->index = index;
        userdata->wrote = false;
        filp->private_data = userdata;
        return 0;
    }
}

static int letras123_close(struct inode *inod, struct file *filp) {
    espacio_t *userdata;
    if (filp->private_data == NULL) return -EPERM;
    userdata = (espacio_t *) filp->private_data;
    //spin_lock(&lock);
    libres[userdata->index] = true;
    //spin_unlock(&lock);

    kfree((void *) userdata);
    filp->private_data = NULL;
    return 0;
}

static ssize_t letras123_read(struct file *filp, char __user *data, size_t s, loff_t *off) {
    espacio_t *userdata;
    int i;
    int index;
    char *buffer;

    if (filp->private_data == NULL) return -EPERM;
    userdata = (espacio_t *) filp->private_data;
    if (!(userdata->wrote)) return -EPERM;

    index = userdata->index;
    buffer = (char *) kmalloc(s, GFP_KERNEL);
    i = 0;
    while (i < s) {
        buffer[i] = espacios[index];
        i++;
    }
    copy_to_user(data, buffer, s);
    kfree((void *) buffer);

    return s;
}

static ssize_t letras123_write(struct file *filp, const char __user *data, size_t s, loff_t *off) {
    char c;
    espacio_t *userdata;
    
    if (filp->private_data == NULL) return -EPERM;
    userdata = (espacio_t *) filp->private_data;
    if (userdata->wrote) return 0;

    copy_from_user(&c, data, 1);
    espacios[userdata->index] = c;
    userdata->wrote = true;

    return 1;
}

static struct file_operations letras123_operaciones = {
	// Completar
	.owner = THIS_MODULE,
    .open = letras123_open,
    .release = letras123_close,
	.read = letras123_read,
	.write = letras123_write
};

static int __init letras123_init(void) {
	//Conseguir los device numbers (el major y el minor)
	alloc_chrdev_region(&major, 0, 1, "letras123_dev"); //firstminor = 0; count = 1
	
    //Inicializar el device como un char device
	cdev_init(&letras123_dev, &letras123_operaciones);	
	//Asignar los números al dispositivo que inicializamos previamente
	cdev_add(&letras123_dev, major, 1); //count = 1
	
	//Crear los nodos correspondientes en el sistema de archivos.
	mi_class = class_create(THIS_MODULE, DEVICE_NAME);
	device_create(mi_class, NULL, major, NULL, DEVICE_NAME);

    spin_lock_init(&lock);
    libres[0] = libres[1] = libres[2] = true;

	return 0;
}

static void __exit letras123_exit(void) {
    //Destruir nodos - Primero
	device_destroy(mi_class, major);
	class_destroy(mi_class);
	//Liberar recursos - Segundo
	unregister_chrdev_region(major, 1); //count = 1
	cdev_del(&letras123_dev);
}

// Completar
module_init(letras123_init);
module_exit(letras123_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Generador de letras");