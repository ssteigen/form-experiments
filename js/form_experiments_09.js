/**
 * Automatic upload behavior for file fields.
 *
 * Copied from AutoUpload module. Slightly modified.
 *
 * @see https://www.drupal.org/project/autoupload
 */
// (function ($) {
//   Drupal.behaviors.fileAutoUpload = {
//     attach: function(context, settings) {
//       $('.auto-upload input.form-submit[value=Upload]', context).hide();
//       $('.auto-upload input.form-file', context).change(function() {
//         var $parent = $(this).closest('.fieldset-wrapper');
//
//         //setTimeout to allow for validation
//         //would prefer an event, but there isn't one
//         setTimeout(function() {
//           if(!$('.error', $parent).length) {
//             $('input.form-submit[value=Upload]', $parent).mousedown();
//           }
//         }, 100);
//       });
//     }
//   };
// })(jQuery);

/**
 * Auto-complete for the provider text field.
 */
(function ($) {
  Drupal.behaviors.autocompleteProviders = {
    attach: function (context, settings) {
      $("#edit-provider", context).autocomplete({
        source: Drupal.settings.form_experiment_09_providers
      });
    }
  };
})(jQuery);
