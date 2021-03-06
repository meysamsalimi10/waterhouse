<?php

/**
 * @file
 * Webform module product grid component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_product_grid() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 0,
    'extra' => array(
      'options' => '',
      'questions' => '',
      'optrand' => 0,
      'qrand' => 0,
      'title_display' => 0,
      'custom_option_keys' => 0,
      'custom_question_keys' => 0,
      'description' => '',
      'attributes' => array(),
      'private' => FALSE,
      'products' => NULL,
    ),
  );
}

// Product Grid depends on functions provided by select.
webform_component_include('select');

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_product_grid() {
  return array(
    'uc_webform_render_product_grid' => array(
      'render element' => 'element',
      'file' => 'components/product_grid.inc',
      'path' => drupal_get_path('module', 'uc_webform'),
    ),
    'uc_webform_display_product_grid' => array(
      'render element' => 'element',
      'file' => 'components/product_grid.inc',
      'path' => drupal_get_path('module', 'uc_webform'),
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function theme_uc_webform_render_product_grid($element) {
  // TODO: Should this theme uc_webform_render_product_grid be declared in
  // hook_theme()?
  $temp_rows = array();
  $columns = array(array(
      'data' => '',
      'class' => 'uc_webform-product-grid-product',
    ));

  $option_ids = array();
  $product_keys = element_children($element['element']);

  // This loop creates the row at the top (which is a list of product options)
  // and the first column on the left (which is the product title column).
  foreach (element_children($element['element']) as $key) {
    $product_element = $element['element'][$key];

    // Create a row with the Product title.
    $row = array(array(
        'data' => $product_element['#title'],
        'class' => 'uc_webform-product-grid-product',
        'product' => "$key",
      ));

    foreach ($product_element['#options'] as $optionid => $opt_val) {
      if (!in_array($optionid, $option_ids)) {
        $columns[] = array(
          'data' => $opt_val,
          'class' => 'checkbox uc_webform-product-grid-option',
          'oid' => $optionid,
        );
      }
      $option_ids[] = $optionid;
    }
    $temp_rows[] = $row;
  }

  $final_rows = array();
  foreach ($temp_rows as $row) {
    $product_key = $row[0]['product'];
    $radios = form_process_radios($element['element'][$product_key]);
    foreach ($columns as $col) {
      if (isset($col['oid'])) {
        if (isset($radios[$col['oid']])) {
          unset($radios[$col['oid']]['#title']);
          $row[] = array(
            'data' => drupal_render($radios[$col['oid']]),
            'class' => 'checkbox webform-grid-option',
          );
        }
        else {
          $row[] = array(
            'data' => 'n/a',
            'class' => 'checkbox webform-grid-option',
          );
        }
      }
    }
    $final_rows[] = $row;
  }

  $option_count = count($columns) - 1;

  $output = theme('table', array(
    'header' => $columns,
    'rows' => $final_rows,
    'attributes' => array(
      'class' => array(
        'uc_webform-product-grid uc_webform-product-grid-' . $option_count,
      ),
    ),
  ));
  $variables['element'] = $element;
  $variables['value'] = $output;
  return $output;

}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_uc_webform_display_product_grid($element) {

  // TODO: Should this theme uc_webform_display_product_grid be declared in
  // hook_theme()?
  $output = '';
  if (isset($element['element']['#value'])) {
    $output = (count($element['element']['#value']) > 1) ? theme('item_list', array('items' => $element['element']['#value'])) : $element['element']['#value'][0];
  }

  return $output;
}

/**
 * Implementation of _webform_edit_component().
 */
function _webform_edit_product_grid($component) {
  $form = array();
  $product_types = uc_product_types();

  // I need to limit the users selection to only those products that *do*
  // contain attributes.
  $attrib_query = db_select('uc_product_attributes', 'pa')
    ->fields('pa', array('nid'));

  $query = db_select('node', 'n')
    ->fields('n', array('nid', 'title'))
    ->condition('n.nid', $attrib_query, 'IN')
    ->condition('n.status', 1, '=');

  $query->join('uc_products', 'p', 'p.nid = n.nid');
  $query->addField('p', 'model');

  $query->orderBy('n.title');

  $products = array();
  foreach ($query->execute() as $product) {
    $products[$product->nid . '_' . check_plain($product->model)] = check_plain($product->title);
  }

  // Most options are stored in the "extra" array, which stores any settings
  // unique to a particular component type.
  $form['extra']['products'] = array(
    '#type' => 'select',
    '#title' => t('Products'),
    '#default_value' => $component['extra']['products'],
    '#multiple' => TRUE,
    '#description' => t('Please select your products. Only products with attributes are displayed.'),
    '#weight' => -3,
    '#size' => 20,
    '#required' => TRUE,
    '#options' => $products,
  );

  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_product_grid($component, $value = NULL) {
  $product_nodes = array();
  $stock_description = "";

  foreach ($component['extra']['products'] as $val) {
    $product_info = explode('_', $val, 2);

    if (module_exists('uc_stock')) {
      $stock_level = uc_stock_level($product_info[1]);
    }
    else {
      $stock_level = FALSE;
    }

    // Check stock levels. The product is only selectable if it is in stock.
    if (($stock_level === FALSE) or (intval($stock_level) > 0)) {
      $product_nodes[$val] = node_load($product_info[0]);
    }
    else {
      $node = node_load($product_info[0]);
      $stock_description .= check_plain($node->title) . ' ' . t('is out of stock.') . '<br />';
    }
  }

  $element = array(
    '#title' => check_plain($component['name']),
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#description' => $component['extra']['description'] . $stock_description,
    '#theme' => 'uc_webform_render_product_grid',
    '#theme_wrappers' => array('webform_element'),
    '#pre_render' => array('webform_element_title_display'),
    '#post_render' => array('webform_element'),
    '#webform_component' => $component,
  );

  // Iterate through each product that will be offered. Products will appear on
  // the left of the grid.
  if(is_array($product_nodes)) {
    foreach ($product_nodes as $key => $product_node) {
      if($product_node) {
        $match = $product_node->nid.'_'.$product_node->model;
        $element[$key] = array(
          '#title' => check_plain($product_node->title),
          '#required' => $component['mandatory'],
          '#type' => 'radios',
          '#default_value' => isset($value[$match]) ? $value[$match] : '',
          '#process' => array('form_process_radios', 'webform_expand_select_ids'),
        );

        // Iterate through each product option for each product. Product options
        // will appear on the top of the grid.
        $product_options = array();
        foreach ($product_node->attributes as $aid => $attribute) {
          foreach ($attribute->options as $option) {
            // aid_oid
            $product_options[$aid . '_' . $option->oid] = check_plain($option->name);
          }
        }
        $element[$key]['#options'] = $product_options;
      }
    }
    if(empty($value)) {
      drupal_add_js(drupal_get_path('module', 'uc_webform') . '/js/deselect.js');
    }
    return $element;
  }
}

/**
 * Implementation of _webform_display_component().
 */
function _webform_display_product_grid($component, $value, $format = 'html') {
  $products = array();
  foreach ($value as $key => $val) {
    $nid_sku = explode('_', $key, 2);
    $aid_oid = explode('_', $val, 2);
    $node = node_load($nid_sku[0]);
    if (!empty($val)) {
      $products[] = t('@title, SKU: @sku, Attribute: @attribute, Option: @option', array(
        '@title' => $node->title,
        '@sku' => $node->model,
        '@attribute' => $node->attributes[$aid_oid[0]]->name,
        '@option' => $node->attributes[$aid_oid[0]]->options[$aid_oid[1]]->name,
      ));
    }
  }

  $element = array(
    '#title' => check_plain($component['name']),
    '#weight' => $component['weight'],
    '#theme' => 'uc_webform_display_product_grid',
    '#component' => $component,
    '#format' => $format,
    '#post_render' => array('webform_element'),
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#value' => $products,
  );

  return $element;
}

/**
 * Implementation of _webform_analysis_component().
 */
function _webform_analysis_product_grid($component, $sids = array()) {

  // Create an entire table to be put into the returned row.
  $rows = array(array());
  $header = array(array());

  $product_nids = array();

  $option_ids = array();

  foreach ($component['extra']['products'] as $product) {
    $nid_sku = explode('_', $product, 2);
    $product_nids[] = $nid_sku[0];
    $product_node = node_load($nid_sku[0]);

    // A nid_sku combination is used as the key here.
    $rows[$product] = array(check_plain($product_node->title));
  }
  // Select the available option IDs and names for each product in the table.
  if (!empty($product_nids)) {
    $query = db_select('uc_product_options', 'po')
      ->fields('po', array('oid'));
    $query->join('uc_attribute_options', 'ao', 'ao.oid = po.oid');
    $query->addField('ao', 'name');
    $query->condition('po.nid', $product_nids, 'IN');
  }
  // Build the header row.
  $options = array();
  foreach ($query->execute() as $option) {
    // Use the oid as an array key to help when quering the
    // webform_submitted_data table (since it stores the oid instead of the
    // option name).
    $header[$option->oid] = check_plain($option->name);
  }

  $query = db_select('webform_submitted_data', 'wsd')
    ->fields('wsd', array('no', 'data'));

  $query->addExpression('COUNT(data)', 'datacount');
  $query->condition('nid', $component['nid'], '=');
  $query->condition('cid', $component['cid'], '=');
  $query->groupBy('no');
  $query->groupBy('data');

  $selections = array();
  foreach ($query->execute() as $data) {
    $aid_oid = explode('_', $data->data, 2);
    // The information is stored in the DB as attribute-ID_option-ID. This line
    // removes the attribute ID and leaves the $selections key as the option ID.
    if (isset($aid_oid[1])) {
      $selections[$data->no][$aid_oid[1]] = $data->datacount;
    }
  }

  foreach ($rows as $rkey => $rval) {
    if ($rkey != 0) {
      foreach ($header as $hkey => $hval) {
        if ($hkey != 0) {
          // $rkey = product-nid_sku.
          // $hkey = option ID.
          $rows[$rkey][] = isset($selections[$rkey][$hkey]) ? $selections[$rkey][$hkey] : 0;
        }
      }
    }
  }

  $rows[0] = $header;
  return array(
    'table_rows' => $rows,
  );
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_product_grid($component, $value) {
  $output = '';

  // Set the value as a single string.
  if(is_array($value)) {
    foreach ($value as $key => $val) {
      $nid_sku = explode('_', $key, 2);
      $aid_oid = explode('_', $val, 2);
      if (isset($nid_sku[0])) {
        $node = node_load($nid_sku[0]);
        // Output the SKU and the option.
        if (isset($aid_oid[0]) && isset($aid_oid[1])) {
          $output .= check_plain($node->model) . ' - ' . check_plain($node->attributes[$aid_oid[0]]->options[$aid_oid[1]]->name) . '<br />';
        }
      }
    }
  }
  return $output;
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_product_grid($component, $export_options) {
  $headers = array(
    0 => array(),
    1 => array(),
    2 => array(),
  );

  if ($export_options['select_format'] == 'separate') {
    $headers[0][] = '';
    $headers[1][] = check_plain($component['name']);

    $count = 0;
    foreach ($component['extra']['products'] as $val) {
      $product_info = explode('_', $val, 2);
      $node = node_load($product_info[0]);
      // Add blank cells.
      if ($count != 0) {
        // Empty column per sub-field in main header.
        $headers[0][] = '';
        $headers[1][] = '';
      }

      // List the product options as header cells.
      $headers[2][] = check_plain($node->title);
      $count++;
    }
  }
  else {
    $headers[0][] = '';
    $headers[1][] = '';
    $headers[2][] = check_plain($component['name']);
  }

  return $headers;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_product_grid($component, $export_options, $value) {

  $data = array();

  // Separate format.
  if ($export_options['select_format'] == 'separate') {
    foreach ($component['extra']['products'] as $product) {
      if (isset($value[$product])) {
        $nid_sku = explode('_', $product, 2);
        $aid_oid = explode('_', $value[$product], 2);

        if (isset($nid_sku[0])) {
          $node = node_load($nid_sku[0]);
          if (isset($aid_oid[0]) && isset($aid_oid[1])) {
            $data[] = check_plain($node->attributes[$aid_oid[0]]->options[$aid_oid[1]]->name);
          } else {
            $data[] = '';
          }
        }
      }
    }
  }
  // Compact format.
  else {
    $data_string;
    foreach ($component['extra']['products'] as $product) {
      if (isset($value[$product])) {
        $nid_sku = explode('_', $product, 2);
        $aid_oid = explode('_', $value[$product], 2);
        if (isset($nid_sku[0])) {
          $node = node_load($nid_sku[0]);
          if (isset($aid_oid[0]) && isset($aid_oid[1])) {
            $data_string .= check_plain($node->attributes[$aid_oid[0]]->options[$aid_oid[1]]->name) . ',';
          }
        }
      }
    }
    $data[] = rtrim($data_string, ',');
  }

  return $data;
}
