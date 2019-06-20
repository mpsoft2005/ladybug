# Diffuse Shading

## N, P, L, V

    N: normal  
    P: point  
    L: light direction  
    V: view direction  

## Lambertian reflectance

https://en.wikipedia.org/wiki/Lambertian_reflectance

C<sub>diffuse</sub> = C<sub>light</sub> * M<sub>diffuse</sub> * dot(N, L)  

C<sub>light</sub>是光源颜色，M<sub>diffuse</sub>是材质的漫反射颜色，N是表面法线，L是指向光源的单位向量

## C++ code sample

```C++
Color diffuse = material->albedo * (light.color * light.intensity) * std::max(0.f, Vector3::Dot(N, L));
```
