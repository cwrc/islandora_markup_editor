define(['jquery', 'jquery-ui', 'cwrcDialogs'], function($, jqueryUi, cwrcDialogs) {

// a bridge between the CWRC-Writer and the cwrcDialogs
return function(writer, config) {
	var w = writer;
	
	var label = config.label;
	var localDialog = config.localDialog;
	var cwrcType = config.cwrcType;
	
	var createEditOpts = {
		success: function(result) {
			if (result.data == null) {
				var error = result.error || 'There was an error creating the entry.';
				w.dialogManager.show('message', {
					title: 'Error',
					msg: error,
					type: 'error'
				});
			} else {
				result = {
					id: 'http://cwrc-dev-01.srv.ualberta.ca/islandora/object/'+result.response.pid
				};
				w.dialogManager.show(localDialog, {
					cwrcInfo: result
				});
			}
		},
		error: function(errorThrown) {
		}
	};
	
	function doEdit(data) {
		cD.popEdit[cwrcType]($.extend({}, createEditOpts, data));
	}
	
	function doCreate() {
		cD.popCreate[cwrcType](createEditOpts);
	}
	
	return {
		show: function(config) {
			if (config.entry) {
				w.dialogManager.show(localDialog, {
					entry: config.entry
				});
			} else {
				var query = w.editor.currentBookmark.rng.toString();
				
				cD.popSearch[cwrcType]({
					query: query,
					success: function(result) {
						if (result.id == null) {
							var id = w.utilities.createGuid();
							if (cwrcType === 'place') {
								result = {
									id: id,
									data: '<geoname><name>Hamilton</name><asciiName>Hamilton</asciiName><lat>44.0501200</lat><lng>-78.2328200</lng><countryCode>CA</countryCode><countryName>Canada</countryName><fcl>A</fcl><fcode>ADM2</fcode><geonameid>'+w.utilities.createGuid()+'</geonameid><granularity>Province/State</granularity></geoname>',
									name: 'Test '+label,
									repository: 'geonames'
								};
							} else {
								result = {
									id: id,
									name: ['Test '+label],
									repository: 'cwrc'
								};
							}
						}
						
						if (result.repository === 'viaf') {
							result.id = 'http://viaf.org/viaf/'+result.id;
						} else if (result.repository === 'geonames') {
							var xmlData = w.utilities.stringToXML(result.data);
							var id = $('geonameid', xmlData).text();
							result.id = 'http://www.geonames.org/'+id;
						} else {
							result.id = 'http://cwrc-dev-01.srv.ualberta.ca/islandora/object/'+result.id;
						}
						
						if ($.isArray(result.name)) {
							result.name = result.name[0];
						}
						
						delete result.data;
						
						w.dialogManager.show(localDialog, {
							query: query,
							cwrcInfo: result
						});
					},
					error: function(errorThrown) {
					},
					buttons: [{
						label : 'Create New '+label,
						action : doCreate
					},{
						label : 'Edit '+label,
						action : doEdit
					}]
				});
			}
		},
		hide: function() {
		}
	};
};

});