<?php

function form_metadata_getdata($field,$lang) {

}

function form_metadata($form_state) {
  global $user;
  $form = array();
  
  $object_type = $_SESSION['meta']['object_type'];
  $arr = $_SESSION['objtype-array']['all'];

  #
  #	English Form
  #

  $form['eng'] = array(
  	'#type' => 'fieldset',
  	'#title' => 'English',
  	'#tree' => TRUE,
  	'#attributes' => array('class' => 'eng-wrapper')
  );

  foreach($arr as $field_name)
  {
  	if (hasfield($object_type, $field_name)) {
  		$desc = $_SESSION['meta']['eng'][$field_name]['description'];
  		if ($_SESSION['meta']['eng'][$field_name]['required'] == true) {
  			$desc .= '<span class="form-required" title="This field is required.">*</span>';
  		}
  			
  		$form['eng'][$field_name] = array(
  			'#type' => $_SESSION['meta']['eng'][$field_name]['type'],
  			'#title' => $desc,
  			'#required' => false, //$_SESSION['meta']['eng'][$field_name]['required'],
  		);

  		if ($_SESSION['meta']['eng'][$field_name]['type'] == 'textfield' || $_SESSION['meta']['eng'][$field_name]['type'] == 'textarea')
  		{
  			$form['eng'][$field_name]['#default_value'] = $_SESSION['meta']['eng'][$field_name]['value'];
  		
  			if ($_SESSION['meta']['eng'][$field_name]['type'] == 'textarea') {
  				$form['eng'][$field_name]['#resizable'] = false;
  			}

  			if (isset($_SESSION['meta']['eng'][$field_name]['length'])) {
  				$form['eng'][$field_name]['#maxlength'] = $_SESSION['meta']['eng'][$field_name]['length'];
  			}
  		}
  		elseif ($_SESSION['meta']['eng'][$field_name]['type'] == 'select')
  		{
  			$form['eng'][$field_name]['#options'] = $_SESSION['meta']['eng'][$field_name]['options'];
  			$form['eng'][$field_name]['#default_value'] = $_SESSION['meta']['eng'][$field_name]['value'];
  			$form['eng'][$field_name]['#multiple'] = $_SESSION['meta']['eng'][$field_name]['multiple'];
  			$form['eng'][$field_name]['#attributes'] = array('class' => 'eng-opt');
  		}
  	}
  }


  #
  #	Arabic Form
  #

  $form['ara'] = array(
  	'#type' => 'fieldset',
  	'#title' => t('العربية'),
  	'#tree' => TRUE,
  	'#attributes' => array('class' => 'ara-wrapper')
  );

  foreach($arr as $field_name)
  {
  	if (hasfield($object_type, $field_name)) {
  		$desc = $_SESSION['meta']['ara'][$field_name]['description'];
  		if ($_SESSION['meta']['ara'][$field_name]['required'] == true) {
  			$desc .= '<span class="form-required" title="This field is required.">*</span>';
  		}

  		$form['ara'][$field_name] = array(
  			'#type' => $_SESSION['meta']['ara'][$field_name]['type'],
  			'#title' => $desc,
  			'#required' => false, //$_SESSION['meta']['ara'][$field_name]['required'],
  		);

  		if ($_SESSION['meta']['ara'][$field_name]['type'] == 'textfield' || $_SESSION['meta']['ara'][$field_name]['type'] == 'textarea')
  		{
  			$form['ara'][$field_name]['#default_value'] = $_SESSION['meta']['ara'][$field_name]['value'];
  			
  			if ($_SESSION['meta']['ara'][$field_name]['type'] == 'textarea') {
  				$form['ara'][$field_name]['#resizable'] = false;
  			}

  			if (isset($_SESSION['meta']['ara'][$field_name]['length'])) {
  				$form['ara'][$field_name]['#maxlength'] = $_SESSION['meta']['ara'][$field_name]['length'];
  			}
  		}
  		elseif ($_SESSION['meta']['ara'][$field_name]['type'] == 'select')
  		{
  			$form['ara'][$field_name]['#options'] = $_SESSION['meta']['ara'][$field_name]['options'];
  			$form['ara'][$field_name]['#default_value'] = $_SESSION['meta']['ara'][$field_name]['value'];
  			$form['ara'][$field_name]['#multiple'] = $_SESSION['meta']['ara'][$field_name]['multiple'];
  			$form['ara'][$field_name]['#attributes'] = array('class' => 'ara-opt');
  		}
  	}
  }

  $form['footing'] = array(
  	'#type' => 'markup',
  	'#value' => '<div style="height: 10px; clear: both;"></div>
  				 <div style="width: 375px; float: left;">PID (Master key): '.getnextpid().'</div>
  				 <div style="width: 300px; float: left; text-align: right;">File Format: '.getcontentmodel().'</div>
  				 <div style="height: 10px; clear: both;"></div>
  				 <div style="width: 375px; float: left;">Document Type: '.getobjecttype().'</div>
  				 <div style="width: 300px; float: left; text-align: right;">Metadata Cataloger: '.$user->name.'</div>
  				 <div style="height: 10px; clear: both;"></div>',
  );

  $form['hiddensubmit'] = array(
  	'#type' => 'submit',
  	'#value' => 'hidden',
  	'#prefix' => '<div style="width: 1px; height: 1px; overflow: hidden;">',	
  	'#suffix' => '</div>',
  );	
  
  $form['clear'] = array(
  	'#type' => 'markup',
  	'#value' => '<input type="reset" value="Clear" />',
  	'#prefix' => '<div style="width: 375px; float: left;">',
  );

  $form['cancel'] = array(
  	'#type' => 'submit',
  	'#value' => 'Cancel',
  	'#validate' => array('form_metadata_cancel'),
  	'#suffix' => '</div>',
  );

  $form['back'] = array(
  	'#type' => 'submit',
  	'#value' => 'Back',
  	'#validate' => array('form_metadata_back'),
  	'#prefix' => '<div style="width: 300px; float: left; text-align: right;">',
  );

  $form['submit'] = array(
  	'#type' => 'submit',
  	'#value' => 'Next',
  	'#suffix' => '</div>',
  );	

  return $form;
}

function form_metadata_cancel($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  drupal_get_messages('error');
  drupal_goto('istir/home');
}

function form_metadata_back($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  drupal_get_messages('error');
  drupal_goto('istir/forms/upload');

}

function form_metadata_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;

  $object_type = $_SESSION['meta']['object_type'];
  $arr = $_SESSION['objtype-array']['all'];
  $flag = false;

  foreach($arr as $field_name) {
  	if (hasfield($object_type, $field_name)) {
  		if ($_SESSION['meta']['eng'][$field_name]['required'] == true) {
  			if ($form_state['values']['eng'][$field_name] == '' && $form_state['values']['ara'][$field_name] == '') {
  				$flag = true;
  				drupal_set_message($_SESSION['meta']['eng'][$field_name]['description'].'/'.$_SESSION['meta']['ara'][$field_name]['description'].' is required.', 'error');
  			}
  		}
  	}
  }


  $form_state['values']['eng']['language'] = $form_state['values']['eng']['language'] != '' ? $form_state['values']['eng']['language'] : null;
  $form_state['values']['ara']['language'] = $form_state['values']['ara']['language'] != '' ? $form_state['values']['ara']['language'] : null;

  $_SESSION['meta']['eng']['title']['value'] = $form_state['values']['eng']['title'];
  $_SESSION['meta']['eng']['creator1']['value'] = $form_state['values']['eng']['creator1'];
  	$_SESSION['meta']['eng']['creator2']['value'] = $form_state['values']['eng']['creator2'];
  	$_SESSION['meta']['eng']['creator3']['value'] = $form_state['values']['eng']['creator3'];
  	$_SESSION['meta']['eng']['creator4']['value'] = $form_state['values']['eng']['creator4'];
  	$_SESSION['meta']['eng']['creator5']['value'] = $form_state['values']['eng']['creator5'];
  	$_SESSION['meta']['eng']['creator6']['value'] = $form_state['values']['eng']['creator6'];
  	$_SESSION['meta']['eng']['creator7']['value'] = $form_state['values']['eng']['creator7'];
  	$_SESSION['meta']['eng']['creator8']['value'] = $form_state['values']['eng']['creator8'];
  	$_SESSION['meta']['eng']['creator9']['value'] = $form_state['values']['eng']['creator9'];
  	$_SESSION['meta']['eng']['creator10']['value'] = $form_state['values']['eng']['creator10'];
  $_SESSION['meta']['eng']['creatoraffil']['value'] = $form_state['values']['eng']['creatoraffil'];
  $_SESSION['meta']['eng']['creatingorg1']['value'] = $form_state['values']['eng']['creatingorg1'];
  	$_SESSION['meta']['eng']['creatingorg2']['value'] = $form_state['values']['eng']['creatingorg2'];
  	$_SESSION['meta']['eng']['creatingorg3']['value'] = $form_state['values']['eng']['creatingorg3'];
  	$_SESSION['meta']['eng']['creatingorg4']['value'] = $form_state['values']['eng']['creatingorg4'];
  	$_SESSION['meta']['eng']['creatingorg5']['value'] = $form_state['values']['eng']['creatingorg5'];
  	$_SESSION['meta']['eng']['creatingorg6']['value'] = $form_state['values']['eng']['creatingorg6'];
  	$_SESSION['meta']['eng']['creatingorg7']['value'] = $form_state['values']['eng']['creatingorg7'];
  	$_SESSION['meta']['eng']['creatingorg8']['value'] = $form_state['values']['eng']['creatingorg8'];
  	$_SESSION['meta']['eng']['creatingorg9']['value'] = $form_state['values']['eng']['creatingorg9'];
  	$_SESSION['meta']['eng']['creatingorg10']['value'] = $form_state['values']['eng']['creatingorg10'];
  $_SESSION['meta']['eng']['abstract']['value'] = $form_state['values']['eng']['abstract'];
  $_SESSION['meta']['eng']['abstract1']['value'] = $form_state['values']['eng']['abstract1'];
  $_SESSION['meta']['eng']['note']['value'] = $form_state['values']['eng']['note'];
  $_SESSION['meta']['eng']['docno']['value'] = $form_state['values']['eng']['docno'];
  $_SESSION['meta']['eng']['docno1']['value'] = $form_state['values']['eng']['docno1'];
  $_SESSION['meta']['eng']['authority']['value'] = $form_state['values']['eng']['authority'];
  $_SESSION['meta']['eng']['keywords1']['value'] = $form_state['values']['eng']['keywords1'];
  	$_SESSION['meta']['eng']['keywords2']['value'] = $form_state['values']['eng']['keywords2'];
  	$_SESSION['meta']['eng']['keywords3']['value'] = $form_state['values']['eng']['keywords3'];
  	$_SESSION['meta']['eng']['keywords4']['value'] = $form_state['values']['eng']['keywords4'];
  	$_SESSION['meta']['eng']['keywords5']['value'] = $form_state['values']['eng']['keywords5'];
  	$_SESSION['meta']['eng']['keywords6']['value'] = $form_state['values']['eng']['keywords6'];
  	$_SESSION['meta']['eng']['keywords7']['value'] = $form_state['values']['eng']['keywords7'];
  	$_SESSION['meta']['eng']['keywords8']['value'] = $form_state['values']['eng']['keywords8'];
  	$_SESSION['meta']['eng']['keywords9']['value'] = $form_state['values']['eng']['keywords9'];
  	$_SESSION['meta']['eng']['keywords10']['value'] = $form_state['values']['eng']['keywords10'];
  $_SESSION['meta']['eng']['language']['value'] = $form_state['values']['eng']['language'];
  $_SESSION['meta']['eng']['publisher']['value'] = $form_state['values']['eng']['publisher'];
  $_SESSION['meta']['eng']['rights']['value'] = $form_state['values']['eng']['rights'];
  $_SESSION['meta']['eng']['dateoriginalyear']['value'] = $form_state['values']['eng']['dateoriginalyear'];
  $_SESSION['meta']['eng']['dateoriginalyear1']['value'] = $form_state['values']['eng']['dateoriginalyear1'];
  $_SESSION['meta']['eng']['dateoriginal']['value'] = $form_state['values']['eng']['dateoriginal'];
  $_SESSION['meta']['eng']['dateoriginal1']['value'] = $form_state['values']['eng']['dateoriginal1'];
  $_SESSION['meta']['eng']['geocartographic']['value'] = $form_state['values']['eng']['geocartographic'];
  $_SESSION['meta']['eng']['geohierarchical']['value'] = $form_state['values']['eng']['geohierarchical'];
  $_SESSION['meta']['eng']['extent']['value'] = $form_state['values']['eng']['extent'];
  $_SESSION['meta']['eng']['conferencename']['value'] = $form_state['values']['eng']['conferencename'];
  $_SESSION['meta']['eng']['journalname']['value'] = $form_state['values']['eng']['journalname'];
  $_SESSION['meta']['eng']['journalno']['value'] = $form_state['values']['eng']['journalno'];
  $_SESSION['meta']['eng']['journaldate']['value'] = $form_state['values']['eng']['journaldate'];
  $_SESSION['meta']['eng']['journalpagerange']['value'] = $form_state['values']['eng']['journalpagerange'];
  $_SESSION['meta']['eng']['journalissn']['value'] = $form_state['values']['eng']['journalissn'];

  $_SESSION['meta']['ara']['title']['value'] = $form_state['values']['ara']['title'];
  $_SESSION['meta']['ara']['creator1']['value'] = $form_state['values']['ara']['creator1'];
  	$_SESSION['meta']['ara']['creator2']['value'] = $form_state['values']['ara']['creator2'];
  	$_SESSION['meta']['ara']['creator3']['value'] = $form_state['values']['ara']['creator3'];
  	$_SESSION['meta']['ara']['creator4']['value'] = $form_state['values']['ara']['creator4'];
  	$_SESSION['meta']['ara']['creator5']['value'] = $form_state['values']['ara']['creator5'];
  	$_SESSION['meta']['ara']['creator6']['value'] = $form_state['values']['ara']['creator6'];
  	$_SESSION['meta']['ara']['creator7']['value'] = $form_state['values']['ara']['creator7'];
  	$_SESSION['meta']['ara']['creator8']['value'] = $form_state['values']['ara']['creator8'];
  	$_SESSION['meta']['ara']['creator9']['value'] = $form_state['values']['ara']['creator9'];
  	$_SESSION['meta']['ara']['creator10']['value'] = $form_state['values']['ara']['creator10'];
  $_SESSION['meta']['ara']['creatoraffil']['value'] = $form_state['values']['ara']['creatoraffil'];
  $_SESSION['meta']['ara']['creatingorg1']['value'] = $form_state['values']['ara']['creatingorg1'];
  	$_SESSION['meta']['ara']['creatingorg2']['value'] = $form_state['values']['ara']['creatingorg2'];
  	$_SESSION['meta']['ara']['creatingorg3']['value'] = $form_state['values']['ara']['creatingorg3'];
  	$_SESSION['meta']['ara']['creatingorg4']['value'] = $form_state['values']['ara']['creatingorg4'];
  	$_SESSION['meta']['ara']['creatingorg5']['value'] = $form_state['values']['ara']['creatingorg5'];
  	$_SESSION['meta']['ara']['creatingorg6']['value'] = $form_state['values']['ara']['creatingorg6'];
  	$_SESSION['meta']['ara']['creatingorg7']['value'] = $form_state['values']['ara']['creatingorg7'];
  	$_SESSION['meta']['ara']['creatingorg8']['value'] = $form_state['values']['ara']['creatingorg8'];
  	$_SESSION['meta']['ara']['creatingorg9']['value'] = $form_state['values']['ara']['creatingorg9'];
  	$_SESSION['meta']['ara']['creatingorg10']['value'] = $form_state['values']['ara']['creatingorg10'];
  $_SESSION['meta']['ara']['abstract']['value'] = $form_state['values']['ara']['abstract'];
  $_SESSION['meta']['ara']['abstract1']['value'] = $form_state['values']['ara']['abstract1'];
  $_SESSION['meta']['ara']['note']['value'] = $form_state['values']['ara']['note'];
  $_SESSION['meta']['ara']['docno']['value'] = $form_state['values']['ara']['docno'];
  $_SESSION['meta']['ara']['docno1']['value'] = $form_state['values']['ara']['docno1'];
  $_SESSION['meta']['ara']['authority']['value'] = $form_state['values']['ara']['authority'];
  $_SESSION['meta']['ara']['keywords1']['value'] = $form_state['values']['ara']['keywords1'];
  	$_SESSION['meta']['ara']['keywords2']['value'] = $form_state['values']['ara']['keywords2'];
  	$_SESSION['meta']['ara']['keywords3']['value'] = $form_state['values']['ara']['keywords3'];
  	$_SESSION['meta']['ara']['keywords4']['value'] = $form_state['values']['ara']['keywords4'];
  	$_SESSION['meta']['ara']['keywords5']['value'] = $form_state['values']['ara']['keywords5'];
  	$_SESSION['meta']['ara']['keywords6']['value'] = $form_state['values']['ara']['keywords6'];
  	$_SESSION['meta']['ara']['keywords7']['value'] = $form_state['values']['ara']['keywords7'];
  	$_SESSION['meta']['ara']['keywords8']['value'] = $form_state['values']['ara']['keywords8'];
  	$_SESSION['meta']['ara']['keywords9']['value'] = $form_state['values']['ara']['keywords9'];
  	$_SESSION['meta']['ara']['keywords10']['value'] = $form_state['values']['ara']['keywords10'];
  $_SESSION['meta']['ara']['language']['value'] = $form_state['values']['ara']['language'];
  $_SESSION['meta']['ara']['publisher']['value'] = $form_state['values']['ara']['publisher'];
  $_SESSION['meta']['ara']['rights']['value'] = $form_state['values']['ara']['rights'];
  $_SESSION['meta']['ara']['dateoriginalyear']['value'] = $form_state['values']['ara']['dateoriginalyear'];
  $_SESSION['meta']['ara']['dateoriginalyear1']['value'] = $form_state['values']['ara']['dateoriginalyear1'];
  $_SESSION['meta']['ara']['dateoriginal']['value'] = $form_state['values']['ara']['dateoriginal'];
  $_SESSION['meta']['ara']['dateoriginal1']['value'] = $form_state['values']['ara']['dateoriginal1'];
  $_SESSION['meta']['ara']['geocartographic']['value'] = $form_state['values']['ara']['geocartographic'];
  $_SESSION['meta']['ara']['geohierarchical']['value'] = $form_state['values']['ara']['geohierarchical'];
  $_SESSION['meta']['ara']['extent']['value'] = $form_state['values']['ara']['extent'];
  $_SESSION['meta']['ara']['conferencename']['value'] = $form_state['values']['ara']['conferencename'];
  $_SESSION['meta']['ara']['journalname']['value'] = $form_state['values']['ara']['journalname'];
  $_SESSION['meta']['ara']['journalno']['value'] = $form_state['values']['ara']['journalno'];
  $_SESSION['meta']['ara']['journaldate']['value'] = $form_state['values']['ara']['journaldate'];
  $_SESSION['meta']['ara']['journalpagerange']['value'] = $form_state['values']['ara']['journalpagerange'];
  $_SESSION['meta']['ara']['journalissn']['value'] = $form_state['values']['ara']['journalissn'];
  
  if ($flag == false) {
  	drupal_goto('istir/forms/summary');
  }
}
