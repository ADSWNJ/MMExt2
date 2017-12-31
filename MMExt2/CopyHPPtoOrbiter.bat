echo on
echo Copying MMExt2 client headers to %OrbiterDir%\orbitersdk\include...
copy /Y MMExt2_Basic*.hpp %OrbiterDir%\orbitersdk\include
copy /Y MMExt2_Advanced*.hpp %OrbiterDir%\orbitersdk\include
if not exist %OrbiterDir%\orbitersdk\include\MMExt2 mkdir %OrbiterDir%\orbitersdk\include\MMExt2
copy MMExt2\*.* %OrbiterDir%\orbitersdk\include\MMExt2

