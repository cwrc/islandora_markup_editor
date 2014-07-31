define(['jquery', 'jquery-ui', 'dialogs/cwrcDialogBridge'], function($, jqueryUi, cwrcDialogBridge) {
	
return function(writer) {
	var w = writer;
	
	var schema = null;
	if (w.initialConfig.cwrcDialogs != null && w.initialConfig.cwrcDialogs.schemas != null) {
		schema = w.initialConfig.cwrcDialogs.schemas.person;
	}
	if (schema == null) {
		schema = 'js/cwrcDialogs/schemas/entities.rng';
	}
	cD.setPersonSchema(schema);
	
	var bridge = new cwrcDialogBridge(w, {
		label: 'Person',
		localDialog: 'tagPerson',
		cwrcType: 'person'
	});
	
	return bridge;
};

});