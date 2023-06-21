#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>

#define DEVICE_NAME "nulo"

static struct cdev nulo_dev;
static struct class *mi_class;
dev_t major;

static ssize_t nulo_read(struct file *filp, char __user *data, size_t s, loff_t *off) {
	return 0;
}

static ssize_t nulo_write(struct file *filp, const char __user *data, size_t s, loff_t *off) {
	return 0;
}

static struct file_operations nulo_operaciones = {
	// Completar
	.owner = THIS_MODULE,
	.read = nulo_read,
	.write = nulo_write
};

static int __init nulo_init(void) {
	//Conseguir los device numbers (el major y el minor)
	alloc_chrdev_region(&major, 0, 1, "nulo_dev"); //firstminor = 0; count = 1
	
	//Inicializar el device como un char device
	cdev_init(&nulo_dev, &nulo_operaciones);
	//Asignar los números al dispositivo que inicializamos previamente
	cdev_add(&nulo_dev, major, 1); //count = 1
	
	//Crear los nodos correspondientes en el sistema de archivos.
	mi_class = class_create(THIS_MODULE, DEVICE_NAME);
	device_create(mi_class, NULL, major, NULL, DEVICE_NAME);
	return 0;
}

static void __exit nulo_exit(void) {
	//Destruir nodos -Primero
	device_destroy(mi_class, major);
	class_destroy(mi_class);
	//Liberar recursos - Segundo
	unregister_chrdev_region(major, 1); //count = 1
	cdev_del(&nulo_dev);
}

// Completar
module_init(nulo_init);
module_exit(nulo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Una suerte de '/dev/null'");