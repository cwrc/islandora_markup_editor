<?php

/**
 * @file
 * This is the template file for the born digital CWRC-Writer view.
 *
 * @TODO: add documentation about file and available variables
 */
?>

<div class="islandora-crited-wrapper">
<input id="full-window-button" type="button" value="Full Window" />
<div id="cwrc_wrapper" class="cwrc_wrapper islandora-crited-iframe-wrapper iframe_replacement" style="width: 100%; height:600px">

      <div id="cwrc-header" class="ui-layout-north">
        <!-- TODO: These are being hidden in the 'startup.js' script.-->
        <div id="page_selector">Loading....</div>
        <div id="header-inner">
          <div class="header-nav">
            <a href="" id="page-prev"></a>
            <a href="" id="page-next"></a>
          </div>
        </div>
        <div id ="pageChange"></div>
        <div id="header_label_wrapper">
          <h1>CWRCWriter</h1>
        </div>
        <div id="headerButtons"></div>
      </div>

      <div class="cwrc ui-layout-west ui-widget-content">
        <div id="westTabs" class="tabs">
          <ul>
            <li><a href="#entities">Entities</a></li>
            <li><a href="#structure">Structure</a></li>
            <li><a href="#relations">Relations</a></li>
            <li id="annotation_tab"><a href="#image-annotations">Image Annotations</a></li>
          </ul>
          <div id="westTabsContent" class="ui-layout-content">
          </div>
        </div>
      </div>

<div id="cwrc-main" class="ui-layout-center">
	<div class="ui-layout-center">
		<form method="post" action="">
			<textarea id="editor" name="editor" class="tinymce"></textarea>
		</form>
	</div>
	<div class="ui-layout-south">
		<div id="southTabs" class="tabs">
			<ul>
				<li><a href="#validation">Validation</a></li>
			</ul>
			<div id="southTabsContent" class="ui-layout-content"></div>
		</div>
	</div>
</div>

<div class="ui-layout-east">
	<!-- Image annotation -->
	<button id="create_annotation" style="visibility: hidden;" class="menu_button">Annotate</button>
	<div class="image-annotation-wrapper">

		<!-- Persist a single player and build new interface to it -->
		<div id="canvas-body-wrapper" style="width: 100%; height: 800px;">
			<div id="canvas-body">


				<!--  At least one visible image needed for GData transport -->
				<div class="shared-canvas-logo" style="font-size: 8pt">
					<img height="25" src="../islandora_markup_editor/img/small-logo.png"
						style="padding: 0px; margin: 0px; border: 0px; border-top: 2px;" />
					Powered by SharedCanvas
				</div>

			</div>
		</div>
	</div>
</div>
<!-- </div> -->
</div>
</div>
