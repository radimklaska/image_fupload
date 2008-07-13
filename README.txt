IMAGE FUpload MODULE
--------------------

Image FUpload (Multiupload) which is an addition to image module, 
gives the ability to every user who is allowed to upload images by 
using image module, to upload multiple images with one simple click. 
All selected images are uploaded via a flash module (swfupload) and are 
automatically processed.
So, there's no need any more to upload image by image. 

Author:
Stefan Smarzly (grandcat)

INSTALL
--------------------

1.) Copy this directory to a suitable modules directory, 
    such as sites/all/modules
2.) Download SWFUpload-Core v2.1.* at http://code.google.com/p/swfupload/ and extract
    the following files from the zip file: swfupload_f9.swf, swfupload.js, swfupload.queue.js
    Put these files into sites/all/modules/image_fupload/swfupload subdirectory
3.) Activate the module in the module list (dependency: image module)
4.) Now, the module is active and can be used at node/add/image, 
    where the original form is overwritten
5.) Access Permissions have to be set by using image module.

UNINSTALL
--------------------

1.) Deactivate Image FUpload at admin/build/modules
2.) Uninstall Image FUpload at admin/build/modules/uninstall