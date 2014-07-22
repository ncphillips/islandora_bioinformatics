<?php
/**
 * @file fraction-table-theme.tpl.php
 *  Creating the fraction assay table via a template because that seemed the easiest.
 *  This template has a couple of functions to help me do that, although perhaps
 *  the should be moved to another location
 *
 *  To anyone who has to work on this in the future, I apologize. This template isn't actually
 *  complicated, just poorly written.
 */

/**
 * One of the methods used in create a table row for a fraction. This
 * method makes sure that the assays are handled in the right order and
 * prints out the name and weight of the assay.
 *
 * @param $fraction
 * An islandora fraction object
 */
function print_fraction_row($fraction) {

  $fraction_url = "/islandora/object/{$fraction['pid']}";
  // this contains the assay abbreviations in the order we'd like
  // them to appear on the form.
  $form_assays = array("P1B", "HC", "HE", "PC", "ARE", "AP", "SA", "EF", "CA", "PA", "MRSA", "VRE", "MD", "MS", "MT", "LO");

  echo "<tr>";
  echo "<td><a href='$fraction_url'>{$fraction['labid']}</a></td>";
  echo "<td>{$fraction['weight']}</td>";

  // loop through each of the assays and print the result
  foreach ($form_assays as $i) {
    print_fraction_result($fraction, $i);
  }
  echo "</tr>";

}

/**
 * Given an assay and a fraction, it will find the result of that assay on that fraction
 * and then print out the appropriately coloured column
 *
 * @param $fraction
 *  An islandora fraction object.
 * @param $assay
 *  The assay result we want to print out on the fraction.
 */
function print_fraction_result($fraction, $assay) {

  if (array_key_exists("fraction", $fraction)){
    if (array_key_exists($assay, $fraction["fraction"])){
      $css_class = ($fraction["fraction"][$assay]["result"]) ? "assay-".$fraction["fraction"][$assay]["result"]:"assay-none";
      if ($css_class == "assay-inactive"){
        echo "<td class='$css_class'>I</td>";
      }else{
        echo "<td class='$css_class'></td>";
      }
    }else{
      echo "<td class='assay-none'>N</td>";
    }
  }else{
    echo "<td class='assay-none'>N</td>";
  }

}
?>
<style>

  .abbr-header{
    font-weight: bold;
    text-decoration: underline;
  }
  .assay-table td{
    padding: 0px;
    width: 10px;
    font-size: 12px;
  }
  .assay-table th{
    font-weight: bold;
    width: 10px;
    font-size: 12px;

    padding: 0px;
  }
  .assay-hit{
    background-color: red;
  }

  .assay-strong{
    background-color: yellow;
  }

  .assay-medium{
    background-color: orange;
  }

  .assay-low{
    background-color: #808080;
  }

  .assay-none{
    background-color: white;
  }

  .assay-inactive{
    background-color: white;
  }
</style>

<div>
  <table class="assay-table">
    <tr>
      <td colspan="4" class="abbr-header">Abbreviation Key</td>
    </tr>
    <tr>
      <td><strong>AP</strong> - Antiproliferative</td>
      <td><strong>ARE</strong> - ARE</td>
      <td><strong>CA</strong> - Candida Albicans</td>
      <td><strong>EF</strong> - Enterococcus faecalis</td>
    </tr>
    <tr>
      <td><strong>HC</strong> - HCT116</td>
      <td><strong>HE</strong> - HELA</td>
      <td><strong>LO</strong> - Lypoxygenase</td>
      <td><strong>MD</strong> - Mycobacterium diernhoferi</td>
    </tr>
    <tr>
      <td><strong>ME</strong> - Mycobacterium smegmatis</td>
      <td><strong>MF</strong> - Mycobacterium terrae</td>
      <td><strong>MRSA</strong> - MRSA</td>
      <td><strong>P1B</strong> - PTB1B</td>
    </tr>
    <tr>
      <td><strong>PA</strong> - Pseudomonas aeruginosa</td>
      <td><strong>PC</strong> - PC3</td>
      <td><strong>SA</strong> - Staphlococcus aureus</td>
      <td><strong>VRE</strong> - VRE</td>
    </tr>
  </table>

  <h3>Fractions</h3>
  <table class="assay-table">
    <tr>
      <td><strong>Color Codes</strong></td>
      <td class="assay-hit">Hit</td>
      <td class="assay-strong">Strong</td>
      <td class="assay-medium">Medium</td>
      <td class="assay-low">Low</td>
      <td class="assay-inactive">Inactive (I)</td>
      <td class="assay-none">None (N)</td>
    </tr>
  </table>

  <table class="assay-table">
    <thead>
      <tr>
        <th>ID</th>
        <th>Weight</th>
        <th>P1B</th>
        <th>HC</th>
        <th>HE</th>
        <th>PC</th>
        <th>ARE</th>
        <th>AP</th>
        <th>SA</th>
        <th>EF</th>
        <th>CA</th>
        <th>PA</th>
        <th>MRSA</th>
        <th>VRE</th>
        <th>MD</th>
        <th>MS</th>
        <th>MT</th>
        <th>LO</th>
      </tr>
    </thead>

    <?php

    if (!empty($variables['fractions'])){
      $found = false;
      foreach ($variables['fractions'] as $fraction) {
        if ($fraction['type'] == 'fraction'){
          $found = true;
          print_fraction_row($fraction);
        }
      }
      if (!$found) {
        echo "<tr><td colspan='18'>There are no fractions associated with this specimen.</td></tr>";

      }
    }else{
      echo "<tr><td colspan='18'>There are no fractions associated with this specimen.</td></tr>";
    }

    ?>
  </table>


  <h3>Compounds</h3>
  <table class="assay-table">
    <thead>
    <tr>
      <th>ID</th>
      <th>Weight</th>
      <th>P1B</th>
      <th>HC</th>
      <th>HE</th>
      <th>PC</th>
      <th>ARE</th>
      <th>AP</th>
      <th>SA</th>
      <th>EF</th>
      <th>CA</th>
      <th>PA</th>
      <th>MRSA</th>
      <th>VRE</th>
      <th>MD</th>
      <th>MS</th>
      <th>MT</th>
      <th>LO</th>
    </tr>
    </thead>

    <?php

    if (!empty($variables['fractions'])){
      $found_c = false;
      foreach ($variables['fractions'] as $fraction) {
        if ($fraction['type'] == 'compound'){
          $found_c = true;
          print_fraction_row($fraction);
        }
      }
      if (!$found_c){
        echo "<tr><td colspan='18'>There are no compounds associated with this specimen.</td></tr>";
      }
    }else{
      echo "<tr><td colspan='18'>There are no compounds associated with this specimen.</td></tr>";
    }

    ?>
  </table>
</div>