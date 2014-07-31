define([
    'jquery',
    'jquery-ui',
    'attributeWidget'
], function($, jqueryUi, AttributeWidget) {
	
return function(writer) {
	var w = writer;
	
	var id = 'person';
	
	var attWidgetInit = false;
	var attributeWidget;
	
	var marcRoles = [{code: '', title: '(none)'},{code:'abr',title:'Abridger'},{code:'acp',title:'Art copyist'},{code:'act',title:'Actor'},{code:'adi',title:'Art director'},{code:'adp',title:'Adapter'},{code:'aft',title:'Author of afterword, colophon, etc.'},{code:'anl',title:'Analyst'},{code:'anm',title:'Animator'},{code:'ann',title:'Annotator'},{code:'ant',title:'Bibliographic antecedent'},{code:'ape',title:'Appellee'},{code:'apl',title:'Appellant'},{code:'app',title:'Applicant'},{code:'aqt',title:'Author in quotations or text abstracts'},{code:'arc',title:'Architect'},{code:'ard',title:'Artistic director'},{code:'arr',title:'Arranger'},{code:'art',title:'Artist'},{code:'asg',title:'Assignee'},{code:'asn',title:'Associated name'},{code:'ato',title:'Autographer'},{code:'att',title:'Attributed name'},{code:'auc',title:'Auctioneer'},{code:'aud',title:'Author of dialog'},{code:'aui',title:'Author of introduction, etc.'},{code:'aus',title:'Screenwriter'},{code:'aut',title:'Author'},{code:'bdd',title:'Binding designer'},{code:'bjd',title:'Bookjacket designer'},{code:'bkd',title:'Book designer'},{code:'bkp',title:'Book producer'},{code:'blw',title:'Blurb writer'},{code:'bnd',title:'Binder'},{code:'bpd',title:'Bookplate designer'},{code:'brd',title:'Broadcaster'},{code:'brl',title:'Braille embosser'},{code:'bsl',title:'Bookseller'},{code:'cas',title:'Caster'},{code:'ccp',title:'Conceptor'},{code:'chr',title:'Choreographer'},{code:'cli',title:'Client'},{code:'cll',title:'Calligrapher'},{code:'clr',title:'Colorist'},{code:'clt',title:'Collotyper'},{code:'cmm',title:'Commentator'},{code:'cmp',title:'Composer'},{code:'cmt',title:'Compositor'},{code:'cnd',title:'Conductor'},{code:'cng',title:'Cinematographer'},{code:'cns',title:'Censor'},{code:'coe',title:'Contestant-appellee'},{code:'col',title:'Collector'},{code:'com',title:'Compiler'},{code:'con',title:'Conservator'},{code:'cor',title:'Collection registrar'},{code:'cos',title:'Contestant'},{code:'cot',title:'Contestant-appellant'},{code:'cou',title:'Court governed'},{code:'cov',title:'Cover designer'},{code:'cpc',title:'Copyright claimant'},{code:'cpe',title:'Complainant-appellee'},{code:'cph',title:'Copyright holder'},{code:'cpl',title:'Complainant'},{code:'cpt',title:'Complainant-appellant'},{code:'cre',title:'Creator'},{code:'crp',title:'Correspondent'},{code:'crr',title:'Corrector'},{code:'crt',title:'Court reporter'},{code:'csl',title:'Consultant'},{code:'csp',title:'Consultant to a project'},{code:'cst',title:'Costume designer'},{code:'ctb',title:'Contributor'},{code:'cte',title:'Contestee-appellee'},{code:'ctg',title:'Cartographer'},{code:'ctr',title:'Contractor'},{code:'cts',title:'Contestee'},{code:'ctt',title:'Contestee-appellant'},{code:'cur',title:'Curator'},{code:'cwt',title:'Commentator for written text'},{code:'dbp',title:'Distribution place'},{code:'dfd',title:'Defendant'},{code:'dfe',title:'Defendant-appellee'},{code:'dft',title:'Defendant-appellant'},{code:'dgg',title:'Degree granting institution'},{code:'dis',title:'Dissertant'},{code:'dln',title:'Delineator'},{code:'dnc',title:'Dancer'},{code:'dnr',title:'Donor'},{code:'dpc',title:'Depicted'},{code:'dpt',title:'Depositor'},{code:'drm',title:'Draftsman'},{code:'drt',title:'Director'},{code:'dsr',title:'Designer'},{code:'dst',title:'Distributor'},{code:'dtc',title:'Data contributor'},{code:'dte',title:'Dedicatee'},{code:'dtm',title:'Data manager'},{code:'dto',title:'Dedicator'},{code:'dub',title:'Dubious author'},{code:'edc',title:'Editor of compilation'},{code:'edm',title:'Editor of moving image work'},{code:'edt',title:'Editor'},{code:'egr',title:'Engraver'},{code:'elg',title:'Electrician'},{code:'elt',title:'Electrotyper'},{code:'eng',title:'Engineer'},{code:'enj',title:'Enacting jurisdiction'},{code:'etr',title:'Etcher'},{code:'evp',title:'Event place'},{code:'exp',title:'Expert'},{code:'fac',title:'Facsimilist'},{code:'fds',title:'Film distributor'},{code:'fld',title:'Field director'},{code:'flm',title:'Film editor'},{code:'fmd',title:'Film director'},{code:'fmk',title:'Filmmaker'},{code:'fmo',title:'Former owner'},{code:'fmp',title:'Film producer'},{code:'fnd',title:'Funder'},{code:'fpy',title:'First party'},{code:'frg',title:'Forger'},{code:'gis',title:'Geographic information specialist'},{code:'his',title:'Host institution'},{code:'hnr',title:'Honoree'},{code:'hst',title:'Host'},{code:'ill',title:'Illustrator'},{code:'ilu',title:'Illuminator'},{code:'ins',title:'Inscriber'},{code:'itr',title:'Instrumentalist'},{code:'ive',title:'Interviewee'},{code:'ivr',title:'Interviewer'},{code:'inv',title:'Inventor'},{code:'isb',title:'Issuing body'},{code:'jud',title:'Judge'},{code:'jug',title:'Jurisdiction governed'},{code:'lbr',title:'Laboratory'},{code:'lbt',title:'Librettist'},{code:'ldr',title:'Laboratory director'},{code:'led',title:'Lead'},{code:'lee',title:'Libelee-appellee'},{code:'lel',title:'Libelee'},{code:'len',title:'Lender'},{code:'let',title:'Libelee-appellant'},{code:'lgd',title:'Lighting designer'},{code:'lie',title:'Libelant-appellee'},{code:'lil',title:'Libelant'},{code:'lit',title:'Libelant-appellant'},{code:'lsa',title:'Landscape architect'},{code:'lse',title:'Licensee'},{code:'lso',title:'Licensor'},{code:'ltg',title:'Lithographer'},{code:'lyr',title:'Lyricist'},{code:'mcp',title:'Music copyist'},{code:'mdc',title:'Metadata contact'},{code:'mfp',title:'Manufacture place'},{code:'mfr',title:'Manufacturer'},{code:'mod',title:'Moderator'},{code:'mon',title:'Monitor'},{code:'mrb',title:'Marbler'},{code:'mrk',title:'Markup editor'},{code:'msd',title:'Musical director'},{code:'mte',title:'Metal-engraver'},{code:'mus',title:'Musician'},{code:'nrt',title:'Narrator'},{code:'opn',title:'Opponent'},{code:'org',title:'Originator'},{code:'orm',title:'Organizer of meeting'},{code:'osp',title:'Onscreen presenter'},{code:'oth',title:'Other'},{code:'own',title:'Owner'},{code:'pan',title:'Panelist'},{code:'pat',title:'Patron'},{code:'pbd',title:'Publishing director'},{code:'pbl',title:'Publisher'},{code:'pdr',title:'Project director'},{code:'pfr',title:'Proofreader'},{code:'pht',title:'Photographer'},{code:'plt',title:'Platemaker'},{code:'pma',title:'Permitting agency'},{code:'pmn',title:'Production manager'},{code:'pop',title:'Printer of plates'},{code:'ppm',title:'Papermaker'},{code:'ppt',title:'Puppeteer'},{code:'pra',title:'Praeses'},{code:'prc',title:'Process contact'},{code:'prd',title:'Production personnel'},{code:'pre',title:'Presenter'},{code:'prf',title:'Performer'},{code:'prg',title:'Programmer'},{code:'prm',title:'Printmaker'},{code:'prn',title:'Production company'},{code:'pro',title:'Producer'},{code:'prp',title:'Production place'},{code:'prs',title:'Production designer'},{code:'prt',title:'Printer'},{code:'prv',title:'Provider'},{code:'pta',title:'Patent applicant'},{code:'pte',title:'Plaintiff-appellee'},{code:'ptf',title:'Plaintiff'},{code:'pth',title:'Patent holder'},{code:'ptt',title:'Plaintiff-appellant'},{code:'pup',title:'Publication place'},{code:'rbr',title:'Rubricator'},{code:'rce',title:'Recording engineer'},{code:'rcd',title:'Recordist'},{code:'rcp',title:'Addressee'},{code:'rdd',title:'Radio director'},{code:'red',title:'Redaktor'},{code:'ren',title:'Renderer'},{code:'res',title:'Researcher'},{code:'rev',title:'Reviewer'},{code:'rpc',title:'Radio producer'},{code:'rps',title:'Repository'},{code:'rpt',title:'Reporter'},{code:'rpy',title:'Responsible party'},{code:'rse',title:'Respondent-appellee'},{code:'rsg',title:'Restager'},{code:'rsp',title:'Respondent'},{code:'rsr',title:'Restorationist'},{code:'rst',title:'Respondent-appellant'},{code:'rth',title:'Research team head'},{code:'rtm',title:'Research team member'},{code:'sad',title:'Scientific advisor'},{code:'sce',title:'Scenarist'},{code:'scl',title:'Sculptor'},{code:'scr',title:'Scribe'},{code:'sds',title:'Sound designer'},{code:'sec',title:'Secretary'},{code:'sgd',title:'Stage director'},{code:'sgn',title:'Signer'},{code:'sht',title:'Supporting host'},{code:'sll',title:'Seller'},{code:'sng',title:'Singer'},{code:'spk',title:'Speaker'},{code:'spn',title:'Sponsor'},{code:'spy',title:'Second party'},{code:'std',title:'Set designer'},{code:'stg',title:'Setting'},{code:'stl',title:'Storyteller'},{code:'stm',title:'Stage manager'},{code:'stn',title:'Standards body'},{code:'str',title:'Stereotyper'},{code:'srv',title:'Surveyor'},{code:'tcd',title:'Technical director'},{code:'tch',title:'Teacher'},{code:'ths',title:'Thesis advisor'},{code:'tld',title:'Television director'},{code:'tlp',title:'Television producer'},{code:'trc',title:'Transcriber'},{code:'trl',title:'Translator'},{code:'tyd',title:'Type designer'},{code:'tyg',title:'Typographer'},{code:'uvp',title:'University place'},{code:'vdg',title:'Videographer'},{code:'wac',title:'Writer of added commentary'},{code:'wal',title:'Writer of added lyrics'},{code:'wam',title:'Writer of accompanying material'},{code:'wat',title:'Writer of added text'},{code:'wdc',title:'Woodcutter'},{code:'wde',title:'Wood engraver'},{code:'wit',title:'Witness'}];
	
	var mode = null;
	var ADD = 0;
	var EDIT = 1;
	
	var currentId = null;
	var currentData = null;
	
	var processData = function() {
		currentData.certainty = $('#'+id+'_certainty input:checked').val();
		currentData.type = $('#'+id+'_type input:checked').val();
		currentData.role = $('#'+id+'_role select').val();
		currentData.attributes = attributeWidget.getData();
		for (var key in currentData) {
			if (currentData[key] == undefined || currentData[key] == '') {
				delete currentData[key];
			}
		}
	};
	
	var onSaveClick = function() {
		processData();
		dialog.dialog('close');
		// Fix to include custom 'Tag As:' value.
		var tag_as_val = $('#'+id+'_tagAs input').attr('value');
		var description_val = $('#'+id+'_describeAs input').attr('value');
		if (currentData != null) {
			currentData.cwrcInfo.name = tag_as_val;
			currentData.cwrcInfo.description = description_val;
		}
		if (mode == EDIT && currentData != null) {
			w.tagger.editEntity(currentId, currentData);
		} else {
			w.tagger.finalizeEntity('person', currentData);
		}
		currentId = null;
		currentData = null;
	};
	
	var initAttributeWidget = function() {
		// TODO determine if person attributes should be re-added, current markup only allows for persName
//		var personAtts = w.utilities.getChildrenForTag({tag: 'person', type: 'attribute', returnType: 'object'});
//		$.map(personAtts, function(val, key) {
//			personAtts[key].parent = 'person';
//		});
		var persNameAtts = w.utilities.getChildrenForTag({tag: 'persName', type: 'attribute', returnType: 'array'});
		for (var i = 0; i < persNameAtts.length; i++) {
			persNameAtts[i].parent = 'persName';
		}
//		var combinedAtts = {};
//		$.extend(combinedAtts, persNameAtts, personAtts);
//
//		var finalAtts = [];
//		for (var key in combinedAtts) {
//			finalAtts.push(combinedAtts[key]);
//		}
//		finalAtts.sort(function(a, b) {
//			if (a.name < b.name) return -1;
//			if (a.name > b.name) return 1;
//			return 0;
//		});
		
		attributeWidget.buildWidget(persNameAtts);
		attWidgetInit = true;
	};
	
	$(document.body).append(''+
	'<div id="'+id+'Dialog" class="annotationDialog">'+
		'<div id="'+id+'_tagAs">'+
			'<p>Tag as:</p>'+
			'<span class="tagAs"></span>'+
		'</div>'+
		'<div id="'+id+'_describeAs">'+
			'<p>Description:</p>'+
			'<span class="tagAs"></span>'+
		'</div>'+
		'<div id="'+id+'_certainty">'+
	    	'<p>This identification is:</p>'+
			'<input type="radio" id="'+id+'_definite" name="'+id+'_id_certainty" value="definite" /><label for="'+id+'_definite">Definite</label>'+
			'<input type="radio" id="'+id+'_reasonable" name="'+id+'_id_certainty" value="reasonably certain" /><label for="'+id+'_reasonable">Reasonably Certain</label>'+
			'<input type="radio" id="'+id+'_probable" name="'+id+'_id_certainty" value="probable" /><label for="'+id+'_probable">Probable</label>'+
			'<input type="radio" id="'+id+'_speculative" name="'+id+'_id_certainty" value="speculative" /><label for="'+id+'_speculative">Speculative</label>'+
	    '</div>'+
	    '<div id="'+id+'_type">'+
		    '<p>Person type:</p>'+
			'<input type="radio" id="'+id+'_real" name="'+id+'_type_certainty" value="real" /><label for="'+id+'_real">Real</label>'+
			'<input type="radio" id="'+id+'_fictional" name="'+id+'_type_certainty" value="fictional" /><label for="'+id+'_fictional">Fictional</label>'+
			'<input type="radio" id="'+id+'_both" name="'+id+'_type_certainty" value="both" /><label for="'+id+'_both">Both</label>'+
	    '</div>'+
	    '<div id="'+id+'_role">'+
		    '<h3>Add role (optional)</h3>'+
	    	'<div>'+
	    		'<select></select>'+
			'</div>'+
	    '</div>'+
	    '<div>'+
		    '<h3>Markup options</h3>'+
		    '<div id="'+id+'_teiParent" style="position: relative; height: 200px;">'+
		    '</div>'+
		'</div>'+
	'</div>'+
	'');
	
	$('#'+id+'_role').accordion({
		heightStyle: 'content',
		animate: false,
		collapsible: true,
		active: false
	});
	
	$('#'+id+'_teiParent').parent().accordion({
		heightStyle: 'content',
		animate: false,
		collapsible: true,
		active: false
	});
	
	var dialog = $('#'+id+'Dialog');
	dialog.dialog({
		title: 'Tag Person',
		modal: true,
		resizable: true,
		dialogClass: 'splitButtons',
		closeOnEscape: false,
		open: function(event, ui) {
			dialog.parent().find('.ui-dialog-titlebar-close').hide();
//			var doc = $(document);
//			dialog.dialog('option', 'width', doc.width() - 100);
//			dialog.dialog('option', 'height', doc.height() - 100);
//			dialog.dialog('option', 'position', { my: "center", at: "center", of: window });
		},
		height: 650,
		width: 400,
		autoOpen: false,
		buttons: [{
			text: 'Cancel',
			click: function() {
				currentData = null;
				currentId = null;
				dialog.dialog('close');
			}
		},{
			text: 'Save',
			id: id+'SaveButton',
			click: onSaveClick
		}]
	});
	
	$('#'+id+'_certainty').buttonset();
	$('#'+id+'_type').buttonset();
	
	var rolesString = '';
	for (var i = 0; i < marcRoles.length; i++) {
		var role = marcRoles[i];
		rolesString += '<option value="'+role.code+'">'+role.title+'</option>';
	}
	$('#'+id+'_role select').html(rolesString);
	
	attributeWidget = new AttributeWidget({writer: w, parentId: id+'_teiParent'});
	
	return {
		show: function(config) {
			mode = config.entry ? EDIT : ADD;
			
			if (attWidgetInit == false) {
				initAttributeWidget();
			}
			
			// reset the form
			attributeWidget.reset();
			$('#'+id+'_certainty input:checked').prop('checked', false).button('refresh');
			$('#'+id+'_type input:checked').prop('checked', false).button('refresh');
			$('#'+id+'_role select').val('');
			$('#'+id+'_role').accordion('option', 'active', false);
			$('#'+id+'_teiParent').parent().accordion('option', 'active', false);
			$('#'+id+'_tagAs span').empty();
			
			// TODO how to handle ADD/EDIT with cwrcInfo
			
			currentData = {};
			if (config.cwrcInfo != null) {
				$('#'+id+'_tagAs span').html('<input type="text" value="' + config.cwrcInfo.name +'"></input>');
				$('#'+id+'_describeAs span').html('<input type="textarea" style="resize:none;" value="' + "" +'"></input>');
				currentData.cwrcInfo = config.cwrcInfo;
			}
			
			if (mode == EDIT) {
				var data = config.entry.info;
				
				currentData.cwrcInfo = data.cwrcInfo;
				$('#'+id+'_tagAs span').html(data.cwrcInfo.name);
				
				currentId = config.entry.props.id;
				
				var showWidget = attributeWidget.setData(data.attributes);
				if (showWidget) {
					$('#'+id+'_teiParent').parent().accordion('option', 'active', 0);
				}
				
				$('#'+id+'_certainty input[value="'+data.certainty+'"]').prop('checked', true).button('refresh');
				$('#'+id+'_type input[value="'+data.type+'"]').prop('checked', true).button('refresh');
				$('#'+id+'_role select').val(data.role);
			}
			
			dialog.dialog('open');
		}
	};
};

});