(function ($) {
  Drupal.behaviors.autocompleteProviders = {
    attach: function (context, settings) {
      $("#edit-provider", context).autocomplete({
        source: Drupal.settings.form_experiment_07_providers
      });
    }
  };
})(jQuery);
