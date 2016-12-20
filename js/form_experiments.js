/**
 * @file
 *
 * JavaScript to support Form Experiments.
 */

(function ($) {
  Drupal.behaviors.managedFileConfirmRemove = {
    attach: function(context, settings) {
      var myButton = $('.form-item input.form-submit[value=Remove]', context);

      if (myButton.length > 0) {
        var myClick = null;

        //get a list of jQuery handlers bound to the click event
        var jQueryHandlers = myButton.data('events').click;

        console.log(jQueryHandlers);

        //grab the first jquery function bound to this event
        $.each(jQueryHandlers,function(i,f) {
          myClick = f.handler;
          return false;
        });

        //unbind the original
        myButton.unbind('click');

        //bind the modified one
        myButton.click(function(){
          if(window.confirm("Are You Sure?")){
            myClick();
          } else {
            return false;
          }
        });
      }
    }
  }
})(jQuery);
