raytracer: raytracer.o vector3.o point3.o ray3.o shape.o sphere.o cylinder.o
	g++ -o raytracer raytracer.o vector3.o point3.o ray3.o shape.o sphere.o cylinder.o

%.o: %.cpp
	g++ -c -o $(@) $(<)

clean:
	rm *.o *.ppm raytracer