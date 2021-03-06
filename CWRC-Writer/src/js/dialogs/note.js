define(['jquery', 'jquery-ui', 'tinymce'], function($, jqueryUi, tinymce) {
	
return function(writer) {
	var w = writer;
	
	var iframe = null;
	var cwrcWriter = null;
	
	var mode = null;
	var ADD = 0;
	var EDIT = 1;
	
	var currentData = null;
	
	$(document.body).append(''+
	'<div id="noteDialog">'+
		'<div style="position: absolute; top: 5px; left: 5px; right: 5px; bottom: 5px; text-align: center;">'+
			'<div id="note_type">'+
				'<input type="radio" id="note_re" name="note_type" value="researchNote" /><label for="note_re" title="Internal to projects">Research Note</label>'+
				'<input type="radio" id="note_scho" name="note_type" value="scholarNote" /><label for="note_scho" title="Footnotes/endnotes">Scholarly Note</label>'+
				'<input type="radio" id="note_ann" name="note_type" value="typeAnnotation" /><label for="note_ann" title="Informal notes">Annotation</label>'+
			'</div>'+
		'</div>'+
		'<div style="position: absolute; top: 35px; left: 5px; right: 5px; bottom: 5px; border: 1px solid #ccc;">'+
			'<iframe style="width: 100%; height: 100%; border: none;"/>'+ // set src dynamically
		'</div>'+
	'</div>');
	
	var note = $('#noteDialog');
	note.dialog({
		modal: true,
		resizable: true,
		closeOnEscape: false,
		open: function(event, ui) {
			$('#noteDialog').parent().find('.ui-dialog-titlebar-close').hide();
		},
		height: 650,
		width: 850,
		autoOpen: false,
		buttons: {
			'Tag Note': function() {
				noteResult();
			},
			'Cancel': function() {
				try {
					cwrcWriter.editor.remove();
					cwrcWriter.editor.destroy();
				} catch (e) {
					// editor wasn't fully initialized
				}
				currentData = null;
				note.dialog('close');
			}
		}
	});
	$('#note_type').buttonset();
	
	function noteResult() {
		tinymce.DOM.counter = iframe.contentWindow.tinymce.DOM.counter + 1;
		
		currentData.type = $('#note_type input:checked').val();
		var content = cwrcWriter.converter.getDocumentContent();
		currentData.content = content;
	
		if (mode == EDIT) {
			w.tagger.editEntity(w.editor.currentEntity, currentData);
		} else {
			w.tagger.finalizeEntity('note', currentData);
		}
		
		cwrcWriter.editor.remove();
		cwrcWriter.editor.destroy();
		currentData = null;
		note.dialog('close');
	};
	
	return {
		show: function(config) {
			iframe = note.find('iframe')[0];
			if (iframe.src == '') {
				iframe.src = 'note.htm';
			}
			
			currentData = {};
			
			mode = config.entry ? EDIT : ADD;
			var prefix = 'Add ';
			if (mode == EDIT) {
				prefix = 'Edit ';
				currentData = config.entry.info;
			}
			
			var title = prefix+'Note';
			note.dialog('option', 'title', title);
			note.dialog('option', 'position', 'center');
			
			var width = $(document).width() * 0.85;
			var height = $(document).height() * 0.85;
			note.dialog('option', 'width', width);
			note.dialog('option', 'height', height);
			
			note.dialog('open');
			
			// hack to get the writer
			function getCwrcWriter() {
				cwrcWriter = iframe.contentWindow.writer;
				if (cwrcWriter == null) {
					setTimeout(getCwrcWriter, 50);
				} else {
					cwrcWriter.event('writerInitialized').subscribe(postSetup);
				}
			}
			
			function postSetup() {
				if (w.schemaManager.schemaId == 'tei') {
					iframe.contentWindow.tinymce.DOM.counter = tinymce.DOM.counter + 1;
					
					cwrcWriter.event('documentLoaded').subscribe(function() {
						// TODO remove forced XML/no overlap
						cwrcWriter.mode = cwrcWriter.XML;
						cwrcWriter.allowOverlap = false;
						
						cwrcWriter.editor.focus();
					});
					
					// in case document is loaded before tree
					cwrcWriter.event('structureTreeInitialized').subscribe(function(tree) {
						setTimeout(tree.update, 50); // need slight delay to get indents working for some reason
					});
					cwrcWriter.event('entitiesListInitialized').subscribe(function(el) {
						setTimeout(el.update, 50);
					});
					
					if (mode == ADD) {
						$('#note_type input:eq(0)').prop('checked', true).button('refresh');
						var noteUrl = w.cwrcRootUrl+'xml/note_tei.xml';
						cwrcWriter.fileManager.loadDocumentFromUrl(noteUrl);
					} else {
						var data = config.entry.info;
						$('#note_type input[value="'+data.type+'"]').prop('checked', true).button('refresh');
						var xmlDoc = cwrcWriter.utilities.stringToXML(data.content);
						if (xmlDoc.firstChild.nodeName === 'note') {
							// remove the annotationId attribute
							xmlDoc.firstChild.removeAttribute('annotationId');
							// insert the appropriate wrapper tags
							var xml = $.parseXML('<TEI><text><body/></text></TEI>');
							xmlDoc = $(xml).find('body').append(xmlDoc.firstChild).end()[0];
						}
						cwrcWriter.fileManager.loadDocumentFromXml(xmlDoc);
					}
				} else {
					alert('Current schema not supported yet!');
				}
			}
			
			getCwrcWriter();
		},
		hide: function() {
			// TODO destroy the CWRC-Writer
			cwrcWriter.editor.remove();
			cwrcWriter.editor.destroy();
			
			note.dialog('close');
			w.editor.focus();
		}
	};
};

});