<?php
$object = $variables['islandora_object'];
$supertypes = islandora_cmodel_supertypes($object->id);
$subtypes = islandora_cmodel_subtypes($object->id);
$rels_away = islandora_cmodel_relationships_away($object->id);
?>


<h2>Types</h2>

<h3>Supertypes</h3>
<table>
<thead>
  <th>Type</th>
</thead>
<tbody>
<?php
  foreach($supertypes as $st_id){
    $st = islandora_cmodel_load($st_id);
    echo "<tr>";
    echo    "<td>{$st->label}</td>";
    echo "</tr>";
  }
?>
</tbody>
</table>

<h3>Subtypes</h3>
<table>
<thead>
  <th>Type</th>
</thead>
<tbody>
<?php
foreach($subtypes as $st_id){
  $st = islandora_cmodel_load($st_id);
  echo "<tr>";
  echo    "<td>{$st->label}</td>";
  echo "</tr>";
}
?>
</tbody>
</table>

<h2>Relationships</h2>
<table>
  <thead>
<!--    <th>Object</th>-->
    <th>Namespace</th>
    <th>Relationship</th>
    <th>Subject</th>
  </thead>
  <tbody>
  <?php
  foreach($rels_away as $rel ){
    echo "<tr>";
//    echo "<td>{$object->id}</td>";
    echo "<td>{$rel['predicate']['alias']}</td>";
    echo "<td>{$rel['predicate']['value']}</td>";
    echo "<td>{$rel['object']['value']}</td>";
    echo "</tr>";
  }
  ?>
  </tbody>
</table>

<h2>Objects</h2>
<table>
<thead>
<th>Label</th>
<th>Type</th>
</thead>
<tbody>
<?
$objects = islandora_object_related_islandora_objects(
  $object->id, //object
  array($object->id), //CModel
  array(FEDORA_MODEL_URI => array('hasModel')) //Relationships
);
foreach($objects as $o){
  $m = islandora_object_parent_model($o->id)[0];
  echo "<tr>";
  echo "<td>{$o->label}</td>";
  echo "<td>{$m}</td>";
  echo "</tr>";
}
?>
</tbody>
</table>