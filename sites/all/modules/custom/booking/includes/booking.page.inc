<?php
/**
 * Created by JetBrains PhpStorm.
 * User: admin
 * Date: 6/9/16
 * Time: 2:19 PM
 * To change this template use File | Settings | File Templates.
 */
/**
 * Implement hook_form()
 * Form add order
 */
function booking_add_form($form, &$form_state, $entity)
{
  global $user;
  if (isset($entity)) {
    $form_state['booking'] = $entity;
  }
  $form['booking'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 0
  );
  $form['booking']['booking_name'] = array(
    '#title' => t('Name booking'),
    '#type' => 'textfield',
    '#default_value' => isset($entity->booking_name) ? $entity->booking_name : '',
    '#require' => true,
  );
  $form['booking']['booking_from'] = array(
    '#type' => 'date_popup', // types 'date_popup', 'date_text' and 'date_timezone' are also supported. See .inc file.
    '#title' => t('Date of department'),
    '#default_value' => isset($entity->booking_from) ? format_date($entity->booking_from, 'custom', 'd/m/Y') : '',
    '#date_format' => 'd/m/Y',
    '#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
    '#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
    '#date_year_range' => '-3:+10', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
    '#attributes' =>array('data'=>isset($entity->booking_from) ? format_date($entity->booking_from, 'custom', 'd/m/Y') : '')
  );

  $form['booking']['booking_to'] = array(
    '#type' => 'date_popup', // types 'date_popup', 'date_text' and 'date_timezone' are also supported. See .inc file.
    '#title' => t('To date'),
    '#default_value' => isset($entity->booking_to) ? date('d/m/Y', $entity->booking_to) : '',
    '#date_format' => 'd/m/Y',
    '#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
    '#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
    '#date_year_range' => '-3:+10', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
    '#attributes' =>array('data'=>isset($entity->booking_from) ? format_date($entity->booking_from, 'custom', 'd/m/Y') : '')
  );
  $form['booking']['booking_long'] = array(
    '#type' => 'checkbox',
    '#title' => t('Long-term rent'),
  );
  $form['booking']['cars_id'] = array(
    '#type' => 'select',
    '#title' => t('Range of vehicle'),
    '#default_value' => isset($entity->cars_id) ? $entity->cars_id : '',
   // '#options' => cars_get_list(),
    '#require' => true,
  );
  $form['booking']['company_id'] = array(
    '#type' => 'select',
    '#title' => t('Company'),
    '#default_value' => isset($entity->company_id) ? $entity->company_id : '',
   // '#options' => company_get_list(),
    '#require' => true,
  );
  $form['booking']['driver'] = array(
    '#title' => t('Driver'),
    '#type' => 'textfield',
    '#default_value' => isset($entity->driver) ? $entity->driver : '',
    '#require' => true,
  );
  $form['booking']['phone'] = array(
    '#title' => t('Phone'),
    '#type' => 'textfield',
    '#default_value' => isset($entity->phone) ? $entity->phone : '',
    '#require' => true,
  );
  $form['booking']['tour_code'] = array(
    '#title' => t('Tour code'),
    '#type' => 'textfield',
    '#default_value' => isset($entity->tour_code) ? $entity->tour_code : '',
    '#require' => true,
  );
  $form['booking']['tour_name'] = array(
    '#title' => t('Name of the tour'),
    '#type' => 'textfield',
    '#default_value' => isset($entity->tour_name) ? $entity->tour_name : '',
    '#require' => true,
  );

  $form['booking']['price'] = array(
    '#title' => t('Total money'),
    '#type' => 'textfield',
    '#default_value' => isset($entity->price) ? $entity->price : '',
    '#require' => true,
  );
  $form['booking']['guide'] = array(
    '#title' => t('Guide'),
    '#type' => 'textfield',
    '#default_value' => isset($entity->guide) ? $entity->guide : '',
    '#require' => true,
  );
  $form['booking']['guide_phone'] = array(
    '#title' => t('Phone Guide'),
    '#type' => 'textfield',
    '#default_value' => isset($entity->guide_phone) ? $entity->guide_phone : '',
    '#require' => true,
  );
  $form['booking']['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => isset($entity->status) ? $entity->status : '',
    '#options' => array(
      1 => 'Work', 0 => 'Inactive', 2 => 'Complete'
    ),
    '#require' => true,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save booking')
  );
  return $form;
}

/**
 * Implement hook_form_validate()
 * validate price for booking
 */

function booking_add_form_validate($form, &$form_state)
{
  if (empty($form_state['values']['booking_name'])) {
    form_set_error('booking_name', t('Please enter a booking name.'));
  }

  if (empty($form_state['values']['company_id'])) {
    form_set_error('company_id', t('Please select a company.'));
  }
  
  if (empty($form_state['values']['tour_code'])) {
    form_set_error('tour_code', t('Please enter tour code'));
  }
  if (booking_get_booking('booking_name', $form_state['values']['booking_name']) && empty($form_state['booking'])) {
    drupal_set_message(t('Booking ' . $form_state['values']['booking_name'] . ' already exists in the system. You should consider when adding.'), 'warning');
  }

}

/**
 * Implement hook_form_submit()
 * Save booking
 */

function booking_add_form_submit($form, &$form_state)
{
  //check is_new or update
  global $user;
  if (isset($form_state['booking']) && !empty($form_state['booking'])) {
    $entity = $form_state['booking'];
    $entity->uid = $user->uid;
    $entity->booking_name = $form_state['values']['booking_name'];
    $entity->booking_from = strtotime($form_state['values']['booking_from'] . ' 00:00:00');
    if(!empty($form_state['values']['booking_long'])){
      $entity->booking_long = $form_state['values']['booking_long'];
    }

    if (!empty($form_state['values']['booking_to'])) {
      $entity->booking_to = strtotime($form_state['values']['booking_to'] . ' 23:59:59');
    }
    if(!empty($form_state['values']['cars_id'])){
      $entity->cars_id = $form_state['values']['cars_id'];
    }
    if(!empty($form_state['values']['driver'])){
      $entity->driver = $form_state['values']['driver'];
    }
    $entity->company_id = $form_state['values']['company_id'];
    if(!empty($form_state['values']['phone'])){
     $entity->phone = $form_state['values']['phone'];
    }
    if(!empty($form_state['values']['tour_code'])){
    $entity->tour_code = $form_state['values']['tour_code'];
    }
    if(!empty($form_state['values']['tour_name'])){
    $entity->tour_name = $form_state['values']['tour_name'];
    }
    if(!empty($form_state['values']['price'])){
    $entity->price = $form_state['values']['price'];
    }
    if(!empty($form_state['values']['guide'])){
    $entity->guide = $form_state['values']['guide'];
    }
    if(!empty($form_state['values']['guide_phone'])){
    $entity->guide_phone = $form_state['values']['guide_phone'];
    }
    $entity->status = $form_state['values']['status'];
    $entity->changed = REQUEST_TIME;
    if (booking_save($entity)) {
      drupal_set_message(t('Booking ' . $form_state['values']['booking_name'] . ' has been updated successfully.'), 'status');
      $form_state['redirect'] = 'booking/manager';
    } else {
      drupal_set_message(t('Booking ' . $form_state['values']['booking_name'] . ' Not updated successfully. Please try again.'), 'error');
    }
  } else {
    $entity = new stdClass();
    $entity->uid = $user->uid;
    $entity->booking_name = $form_state['values']['booking_name'];
    $entity->booking_from = strtotime($form_state['values']['booking_from'] . ' 00:00:00');
    if(!empty($form_state['values']['booking_long'])){
      $entity->booking_long = $form_state['values']['booking_long'];
    }

    if (!empty($form_state['values']['booking_to'])) {
      $entity->booking_to = strtotime($form_state['values']['booking_to'] . ' 23:59:59');
    }
    if(!empty($form_state['values']['cars_id'])){
      $entity->cars_id = $form_state['values']['cars_id'];
    }
    if(!empty($form_state['values']['driver'])){
      $entity->driver = $form_state['values']['driver'];
    }
    $entity->company_id = $form_state['values']['company_id'];
    if(!empty($form_state['values']['phone'])){
      $entity->phone = $form_state['values']['phone'];
    }
    if(!empty($form_state['values']['tour_code'])){
      $entity->tour_code = $form_state['values']['tour_code'];
    }
    if(!empty($form_state['values']['tour_name'])){
      $entity->tour_name = $form_state['values']['tour_name'];
    }
    if(!empty($form_state['values']['price'])){
      $entity->price = $form_state['values']['price'];
    }
    if(!empty($form_state['values']['guide'])){
      $entity->guide = $form_state['values']['guide'];
    }
    if(!empty($form_state['values']['guide_phone'])){
      $entity->guide_phone = $form_state['values']['guide_phone'];
    }
    $entity->status = $form_state['values']['status'];
    $entity->changed = REQUEST_TIME;
    $entity->created = REQUEST_TIME;
    if (booking_save($entity)) {
      drupal_set_message(t('Booking ' . $form_state['values']['booking_name'] . 'was created successfully.'), 'status');
      $form_state['redirect'] = 'booking/manager';
    } else {
      drupal_set_message(t('Booking ' . $form_state['values']['booking_name'] . ' was not created successfully. Please try again.'), 'error');
    }
  }

}

/**
 * Implement hook_form()
 * Form delete booking
 */
function booking_delete_form($form, &$form_state, $entity)
{
  $form_state['booking'] = $entity;
  $form['title'] = array(
    '#markup' => t('<h3>Do you want to delete @title? ?</h3>', array('@title' => $entity->booking_name)),
  );
  $form['cancel'] = array(
    '#markup' => '<div class="cancel-link">' . l('Cancel', 'booking/manager', array('attributes' => array('id' => 'edit-cancel'))) . '</div>'
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete')
  );
  return $form;

}

/**
 * Implement hook_form_submit
 * Delete drink
 */
function booking_delete_form_submit($form, &$form_state)
{
  $entity = $form_state['booking'];
  booking_delete($entity->id);
  drupal_set_message(t('@title has been successfully deleted.', array('@title' => $entity->booking_name)), 'status');
  $form_state['redirect'] = 'booking/manager';
}

/**
 * Implement add form create tai xe
 */
function booking_driver_form($form, &$form_state){
  $form['driver'] = array(
    '#title' =>t('Select car'),
    '#type'=>'select',
    '#options'=>cars_get_cars_number(),
    '#require'=>true
  );
  $form['pass'] = array(
    '#type' => 'password',
    '#maxlength' => 64,
    '#size' => 15,
    '#require'=>true
  );
  $form['pass'] = array(
    '#type' => 'password_confirm',
    '#size' => 25,
    '#require'=>true
  );
  $form['submit'] = array(
    '#type'=>'submit',
    '#value'=>'Create a driver'
  );
  return $form;
}

/**
 * Implement hook_form_validate
 * validate driver user
 */
function booking_driver_form_validate($form, &$form_state){
  if(empty($form_state['values']['driver'])){
    form_set_error('driver',t('Please select vehicle number'));
  }
  if(empty($form_state['values']['pass'])){
    form_set_error('pass',t('Please enter a password'));
  }
}
/**
 * Implement hook_form_submit
 * create driver account
 */
function booking_driver_form_submit($form, &$form_state){
  $user = user_load_by_name($form_state['values']['driver']);
  if(isset($user) && $user->uid > 0){
    drupal_set_message(t('The driver has already existed. Please go to section <a href="/account/manager">user management</a> to update information.'));
    return;
  }
  $new_user = array(
    'name' => $form_state['values']['driver'],
    'pass' => $form_state['values']['pass'], // note: do not md5 the password
    'mail' => $form_state['values']['driver'].'@car.com',
    'status' => 1,
    'init' => $form_state['values']['driver'].'@car.com',
    'roles' => array(
      DRUPAL_AUTHENTICATED_RID => 'authenticated user',
      6 => 'driver',
    ),
  );

// The first parameter is sent blank so a new user is created.
  user_save('', $new_user);
  drupal_set_message(t('Create a successful driver account.'));
}
