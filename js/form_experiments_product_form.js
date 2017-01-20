/**
 * @file
 * Disable form elements during ajax requests.
 */
// (function ($) {
//   $(document)
//     .ajaxStart(function() {
//       if ($('.ajax-disabling').length) {
//         // Disable elements.
//         $(".ajax-disabling .btn").attr("disabled", "disabled");
//         $(".ajax-disabling .form-file").attr("disabled", "disabled");
//
//         $(".ajax-disabling .btn").addClass("disabled");
//         $(".ajax-disabling .form-file").addClass("disabled");
//       }
//     })
//     .ajaxComplete(function() {
//       if ($('.ajax-disabling').length) {
//         // Enable elements.
//         $(".ajax-disabling .btn").removeAttr("disabled");
//         $(".ajax-disabling .form-file").removeAttr("disabled");
//
//         $(".ajax-disabling .btn").removeClass("disabled");
//         $(".ajax-disabling .form-file").removeClass("disabled");
//       }
//     });
//
// })(jQuery);

/**
 * @file
 * Automatic upload behavior for file fields.
 *
 * Copied from AutoUpload module. Slightly modified.
 *
 * @see https://www.drupal.org/project/autoupload
 */
// (function ($) {
//   Drupal.behaviors.fileAutoUpload = {
//     attach: function(context, settings) {
//       // $('.auto-upload .form-submit[value=Upload]', context).hide();
//
//       // If this field was previously in an error state, clear the error.
//       $('.auto-upload input.form-file', context).click(function() {
//         $(this).removeClass('error')
//       });
//
//       // When the file field changes, click the upload button.
//       $('.auto-upload input.form-file', context).change(function() {
//         var parent = $(this).closest('.auto-upload');
//
//         // Set timeout to allow for validation.
//         // Would prefer an event, but there isn't one.
//         setTimeout(function() {
//           if(!$('.error', parent).length) {
//             $('.form-submit[value=Upload]', parent).mousedown();
//           }
//         }, 100);
//       });
//     }
//   };
// })(jQuery);

/**
 * @file
 * Auto-complete for the provider text field.
 */

(function ($) {
  Drupal.behaviors.fileConfirmRemove = {
    attach: function(context, settings) {
      // To start, hide the two confirmation buttons.
      $('button.confirm-remove', context).hide();
      $('a.cancel-remove', context).hide();

      // When the remove button is clicked,
      // hide it and show the confirmation buttons.
      $('a.remove', context).click(function() {
        if ($(this).is('[disabled=disabled]') != true) {
          $(this).hide();
          var parent = $(this).closest('.form-group');
          $('button.confirm-remove', parent).show();
          $('a.cancel-remove', parent).show();
        }
      });

      // When the cancel button is clicked,
      // hide the confirmation buttons and show the remove button.
      $('a.cancel-remove', context).click(function() {
        if ($(this).is('[disabled=disabled]') != true) {
          $(this).hide();
          var parent = $(this).closest('.form-group');
          $('button.confirm-remove', parent).hide();
          var grandparent = parent.parent('.form-group');
          $('a.remove', grandparent).show();
        }
      });
    }
  };
})(jQuery);
