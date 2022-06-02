raytracer: raytracer.o vector3.o point3.o ray3.o shape.o sphere.o cylinder.o color.o material.o light.o camera.o triangle.o texture.o texCoords.o
	g++ -o raytracer raytracer.o vector3.o point3.o ray3.o shape.o sphere.o cylinder.o color.o material.o light.o camera.o triangle.o texture.o texCoords.o

%.o: %.cpp
	g++ -c -o $(@) $(<)

clean:
	rm *.o out.ppm raytracer