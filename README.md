# Sistemas Empotrados de Alto Desempeño (MP6171)

## Proyecto 1

- Fernando Paris
- Esteban Rivera
- Kevin Viquez

## Instrucciones de uso

1. Clonar el repositorio
`git clone https://github.com/EstebanRiv/g2_hpes_2019.git`

2. Copiar yocto layer **meta-tec** en la misma carpeta donde se encuentra **meta-rpi**

Receta       | Comando           | Descripcion
-------------|-------------------|--------------
tec-image.bb | bitbake tec-image | Genera imagen 
rgb2yuv-c_1.0.bb | bitbake rgb2yuv-c | Crea paquete con binario para convertir imagenes .rgb a .yuv e instala documentacion en "/usr/share/rgb2yuv_c"
rgb2yuv-intrinsics_1.0.bb | bitbake rgb2yuv-intrinsics | Crea paquete con binario para convertir imagenes .rgb a .yuv (utiliza intrisics NEON para mejorar el desempeño) e instala documentacion en "/usr/share/rgb2yuv_intrinsics"
