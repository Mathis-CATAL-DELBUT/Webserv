document.getElementById("redirectForm").addEventListener("submit", function(event)
{
    event.preventDefault();
    var url = document.getElementById("urlInput").value;
    if (url !== "")
      window.location.href = url;
});
  