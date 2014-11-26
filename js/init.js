(function ($) {
  Drupal.behaviors.CwrcFullWindow = {
    attach: function (context, settings){
      $('#full-window-button').click(function() {
        $('.islandora-crited-wrapper').toggleClass('islandora-crited-fullwindow');
        if ($(this).val() == Drupal.t('Full Window')) {
            $('#admin-menu-wrapper').hide();
            $(this).val(Drupal.t('Exit Full Window'));
            $('#cwrc_wrapper').css({
              height: '100%',
            });
        }
        else {
            $(this).val(Drupal.t('Full Window'));
            $('#admin-menu-wrapper').show();
            $('#cwrc_wrapper').css({
                height: '600',
              });
        }
        $('#cwrc_wrapper').layout().resizeAll();
      });
    }
  };
  // Listen for the 'esc' key.
  $(document).keyup(function(e) {
    if (e.keyCode == 27) {
      if($('#full-window-button').val() == Drupal.t('Exit Full Window')) {
        $('.islandora-crited-wrapper').toggleClass('islandora-crited-fullwindow');
        $('#cwrc_wrapper').css({
            height: '600',
          });
        $('#cwrc_wrapper').layout().resizeAll();
        $('#full-window-button').val(Drupal.t('Full Window'));
      }
    }
  });
})(jQuery);