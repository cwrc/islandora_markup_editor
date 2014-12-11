var dsid = "OBJ";                                                               

//require.config({baseUrl: 'js'});
require.config({baseUrl: window.location.protocol + "//" + window.location.hostname + "/sites/all/libraries/CWRC-Writer/src/js"});

  
// and the 'jquery-private' module, in the
// jquery-private.js file:
define('jquery-private', ['jquery'], function ($) {
  return $.noConflict(true);
});

require(['jquery', 'knockout'], function($, knockout) {
  window.ko = knockout; // requirejs shim isn't working for knockout
  
  require(['writer',
            'delegator',
            'jquery.layout',
            'jquery.tablayout'
  ], function(Writer, Delegator) {
      $(function() {
          cwrcWriterInit.call(window, $, Writer, Delegator);
      });
  });
});


function cwrcWriterInit($, Writer, Delegator) {

  // hide various interface elements
  $('#page_selector').hide();
  $('#header-inner').hide();
  $('#pageChange').hide();
  $('#header_label_wrapper').hide();
  $('#annotation_tab').hide();

  cwrc_params = {};
  
  writer = null;

  // Drupal Specific
  console.log(Drupal.settings.islandora_markup_editor.schema_object['schemas']);
  PID = Drupal.settings.islandora_markup_editor.page_pid;
  window.location.hash = '#' + PID;


  // initialize CWRC-Writer
  function doInit(config) {

    writer = new Writer(config);
    writer.init('editor');
    writer.event('writerInitialized').subscribe(function(writer) {
    // load modules then do the setup
    require(['modules/entitiesList','modules/relations','modules/selection',
            'modules/structureTree','modules/validation'
            ], function(EntitiesList, Relations, Selection, StructureTree, Validation) {
              setupLayoutAndModules(writer, EntitiesList, Relations, Selection, StructureTree, Validation);
              writer.fileManager.loadInitialDocument(window.location.hash);
          });
      });
  }

  function doResize() {
    var uiHeight = $('#'+writer.editor.id+'_tbl tr.mceFirst').outerHeight() + 2;
    writer.editor.theme.resizeTo($(window).width(), $(window).height() - uiHeight);
  }


  // read configuration and setup CWRC-WRiter
  (function() {
      moduleUrl = Drupal.settings.basePath;

      url =  Drupal.settings.basePath + Drupal.settings.islandora_markup_editor.page_setup + PID;

      $.ajax({
        url: url, 
        timeout: 3000,
        success: function(data, status, xhr) {
            console.log("Success");
            var config = data;
            //TODO: add path to lib - islandora_markup_editor_add_markup_editor_drupal_settings
            config.cwrcRootUrl = window.location.protocol + "//" + window.location.hostname + "/sites/all/libraries/CWRC-Writer/src/";
            config.project = data; //TODO: is needed by Islandora?
            config.id = 'editor';
            config.delegator = CustomDelegator;
            doInit(config);
            $(window).on('resize', doResize);
        },
        error: function() {
            console.log("failure");
            alert('Error loading ' + url);
        }
      });
  }());
};

