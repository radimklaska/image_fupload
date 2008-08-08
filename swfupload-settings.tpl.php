<?php
// $Id$
?>
        var swfu;
        var jsTimer = false;
        var error_send = false; // Indicates whether swfUpload has already been stopped because of a form error
        var upload_complete = false; // all queued files uploaded?         

		window.onload = function() {
			var settings = {
				flash_url : "<?php print $modulepath; ?>/swfupload/swfupload_f9.swf",
				upload_url: "<?php print $uploadpath; ?>",	// Relative to the SWF file
                post_params: {"PHPSESSID" : "<?php print $sessionid; ?>"},
                file_post_name: "Filedata",
				file_size_limit : "<?php print $maxfilesize; ?>",
				file_types : "*.jpg; *.jpeg; *.png; *.gif",
				file_types_description : "Only images",
				file_upload_limit : "<?php print $uploadlimit; ?>",
				file_queue_limit : "0",
				custom_settings : {
					progressTarget : "fsUploadProgress",
					cancelButtonId : "btnCancel"
				},
				debug: false,

				// The event handler functions are defined in handlers.js
				file_dialog_start_handler : fileDialogStart,
                file_queued_handler : fileQueued,
				file_queue_error_handler : fileQueueError,
				file_dialog_complete_handler : fileDialogComplete,
				upload_start_handler : uploadStart,
				upload_progress_handler : uploadProgress,
				upload_error_handler : uploadError,
				upload_success_handler : uploadSuccess,
				upload_complete_handler : uploadComplete,
				queue_complete_handler : queueComplete	// Queue plugin event
			};            
			swfu = new SWFUpload(settings);	     
          
         };
         
         function startUploadProcess() {
             if (!upload_complete) {  
                // Reset all variables and indicators
                error_send = false;
                if (isNaN(document.getElementById('form_errors')))
                    document.getElementById('form_errors').value = '0';
                if (isNaN(document.getElementById('num_queued_images')))
                    document.getElementById('num_queued_images').value = '1'; // Pretend that there's at least one image in queue so that it works               
            
                // hey, let's go =)
                swfu.startUpload();  
             } else {
                 // Provide a second step to be able to edit captions of image if supported
                 var second_step_url = "<?php print $second_step_url; ?>";
                 if (second_step_url != "") {
                     window.location.href = second_step_url;                     
                 }    
             }            
         }
         
         function UploadComplete(numFilesUploaded) {
             // Provide a second step to be able to edit captions of image if supported             
             var second_step_url = "<?php print $second_step_url; ?>";
             if (second_step_url != "") {
                 upload_complete = true;
                 document.getElementById('btnSelect').disabled = true;
                 window.setTimeout("document.getElementById('startuploadbutton').value = Drupal.t('Next step');document.getElementById('divStatus').innerHTML = (Drupal.formatPlural(" + numFilesUploaded + ", '1 file uploaded in queue.', '@count files uploaded in queue.') + ' ' + Drupal.t('Enter the next step to be able to edit all captions.'))", 1500);
             }
         }