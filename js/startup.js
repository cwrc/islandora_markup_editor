var dsid = "OBJ";                                                               

function cwrcWriterInit($, Writer, Delegator) {

  $('#page_selector').hide();
  $('#header-inner').hide();
  $('#pageChange').hide();
  $('#header_label_wrapper').hide();
  $('#annotation_tab').hide();

  cwrc_params = {};
  
  writer = null;

  console.log(Drupal.settings.islandora_markup_editor.schema_object['schemas']);
  PID = Drupal.settings.islandora_markup_editor.page_pid;
  window.location.hash = '#' + PID;


  function doInit(config) {
    writer = new Writer(config);
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

