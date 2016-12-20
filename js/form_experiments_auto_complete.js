/**
 * Auto-complete for the provider text field.
 */
(function ($) {
  Drupal.behaviors.autocompleteProviders = {
    attach: function (context, settings) {
      $("#edit-provider", context).autocomplete({
        source: Drupal.settings.form_experiment_providers
      });
    }
  };
})(jQuery);
