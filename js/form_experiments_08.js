/**
 * Automatic upload behavior for file fields.
 *
 * Copied from AutoUpload module. Slightly modified.
 *
 * @see https://www.drupal.org/project/autoupload
 */
(function ($) {
  Drupal.behaviors.fileAutoUpload = {
    attach: function(context, settings) {
      $('.auto-upload input.form-submit[value=Upload]', context).hide();

      // If this field was previously in an error state, clear the error.
      $('.auto-upload input.form-file', context).click(function() {
        $(this).removeClass('error')
      });

      // When the file field changes, click the upload button.
      $('.auto-upload input.form-file', context).change(function() {
        var $parent = $(this).closest('.fieldset-wrapper');

        // Set timeout to allow for validation.
        // Would prefer an event, but there isn't one.
        setTimeout(function() {
          if(!$('.error', $parent).length) {
            $('input.form-submit[value=Upload]', $parent).mousedown();
          }
        }, 100);
      });
    }
  };
})(jQuery);

/**
 * Auto-complete for the provider text field.
 */
(function ($) {
  Drupal.behaviors.autocompleteProviders = {
    attach: function (context, settings) {
      $("#edit-provider", context).autocomplete({
        source: Drupal.settings.form_experiment_08_providers
      });
    }
  };
})(jQuery);
