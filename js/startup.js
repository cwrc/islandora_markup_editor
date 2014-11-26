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
        config.id = 'editor';
        config.delegator = CustomDelegator;
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
        var configXHR = $.ajax({url: 'http://cwrc-dev-01.srv.ualberta.ca/sites/all/libraries/CWRC-Writer/src/js/writerConfig.js', dataType: 'json'});
        var projectXHR = $.ajax({url: 'http://apps.testing.cwrc.ca/editor/documents/info/projectname'});
        $.when(
            configXHR
            , projectXHR
        ).then(function(config) {
            config = config[0];
            project = 'editor';
            config.project = project;
            moduleUrl = Drupal.settings.basePath;
            config.cwrcRootUrl = window.location.protocol+'//'+window.location.host+window.location.pathname.replace(window.location.pathname.split('/').pop(),'');
            doInit(config);
        },function(xhr) {
            if (configXHR.state() === 'resolved') {
                var config = $.parseJSON(configXHR.responseText);
                //config.cwrcRootUrl = window.location.protocol+'//'+window.location.host+window.location.pathname.replace(window.location.pathname.split('/').pop(),'');
                config.cwrcRootUrl = window.location.protocol + "//" + window.location.hostname + "/sites/all/libraries/CWRC-Writer/src/";
                //

                url =  Drupal.settings.basePath + Drupal.settings.islandora_markup_editor.page_setup + PID;

                $.ajax({
                    url: url, 
                    timeout: 3000,
                    success: function(data, status, xhr) {
                        console.log("Success");
                        config.project = data;
                        $(window).on('resize', doResize);
                    },
                    error: function() {
                        console.log("failure");
                    }
                });



                doInit(config);
            } else {
                alert('Error loading writerConfig.js');
            }
        });

    }());
};

